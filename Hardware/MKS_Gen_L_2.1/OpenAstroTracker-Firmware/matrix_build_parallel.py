"""
matrix_build.py를 병렬화하는 방법과 관련된 모든 기능이 있는 모듈입니다.
완벽한 분리는 아니지만, '매트릭스' 로직과 '빌드 방법' 로직을 분리하는 데 도움이 됩니다.
"""
import os
import shutil
import subprocess
import tempfile
import time
from pathlib import Path
from typing import Optional, List
from dataclasses import dataclass


@dataclass
class Executor:
    """
    빌드 중인 솔루션을 정의하는 핵심 데이터
    """
    # 솔루션을 빌드하는 디렉토리
    proj_dir: Path
    # 솔루션 딕셔너리
    solution: Optional[dict] = None
    # 솔루션을 빌드하는 프로세스
    proc: Optional[subprocess.Popen] = None
    # 나중에 정리할 수 있도록 tempdir 데이터를 보관하는 객체
    tempdir_obj: Optional[tempfile.TemporaryDirectory] = None


def generate_config_file(project_location: Path, flag_values: dict):
    content = "#pragma once\n\n"
    for key, value in flag_values.items():
        content += "#define {} {}\n".format(key, value)

    with open(Path(project_location, "Configuration_local_matrix.hpp"), 'w') as f:
        f.write(content)
        f.flush()


def execute(project_location: Path, board: str, flag_values: dict, jobs: int = 1, out_pipe=True) -> subprocess.Popen:
    """
    솔루션을 빌드하는 실행기를 시작
    :param project_location: 솔루션을 빌드할 디렉토리
    :param board: 보드 유형(환경)
    :param flag_values: 설정 파일을 생성할 #define 딕셔너리
    :param jobs: 빌드 프로세스가 사용할 작업 수
    :param out_pipe: 실행기의 stdout/stderr를 파이프로 할지 여부
    :return: 솔루션을 실행하는 프로세스 객체
    """
    build_env = dict(os.environ)
    build_env["PLATFORMIO_BUILD_FLAGS"] = "-DMATRIX_LOCAL_CONFIG=1"
    generate_config_file(project_location, flag_values)

    proc = subprocess.Popen(
        ['pio',
         'run',
         f'--project-dir={str(project_location.resolve())}',
         f'--environment={board}',
         f'--jobs={jobs}',
         ],
        stdout=subprocess.PIPE if out_pipe else None,
        stderr=subprocess.PIPE if out_pipe else None,
        env=build_env,
        close_fds=True,
    )
    return proc


def get_available_executor_idx(e_list: List[Executor]) -> Optional[int]:
    """
    유휴 실행기의 인덱스 가져오기
    :param e_list: 실행기 목록
    :return: 유휴 실행기 인덱스, 모두 사용 중이면 None
    """
    for i, executor in enumerate(e_list):
        if executor.proc is None:
            return i
    return None


def get_finished_executor_idx(e_list: List[Executor]) -> Optional[int]:
    """
    완료된 실행기의 인덱스 가져오기
    :param e_list: 실행기 목록
    :return: 완료된 실행기 인덱스, 모두 사용 중이면 None
    """
    for i, executor in enumerate(e_list):
        if executor.proc is not None and executor.proc.poll() is not None:
            return i
    return None


def cleanup_tempdirs(e_list: List[Executor]):
    """
    실행기가 사용한 모든 임시 디렉토리 삭제
    :param e_list: 실행기 목록
    """
    for executor in e_list:
        if executor.tempdir_obj is not None:
            tempdir_path = executor.tempdir_obj.name
            print(f'Deleting {tempdir_path}')
            shutil.rmtree(tempdir_path, ignore_errors=True)


def create_executors(num_executors: int, local_paths_to_link: List[Path]) -> List[Executor]:
    """
    실행기와 관련 임시 디렉토리를 생성하고 필요한 모든 프로젝트 파일을 소프트 링크
    :param num_executors: 생성할 실행기 수
    :param local_paths_to_link: 소프트 링크할 파일 목록
    :return: 실행기 목록
    """
    executor_list: List[Executor] = []
    print(f'Creating {num_executors} executors')
    for executor_idx in range(num_executors):
        tempdir = tempfile.TemporaryDirectory()
        temp_proj_path = Path(tempdir.name)
        for local_path in local_paths_to_link:
            temp_dst_path = Path(temp_proj_path, local_path).resolve()
            os.makedirs(temp_dst_path.parent, exist_ok=True)
            os.symlink(local_path.resolve(), temp_dst_path)
        executor_list.append(Executor(temp_proj_path, tempdir_obj=tempdir))
        print(f'{executor_idx} ', end='')
    print()
    return executor_list


def copy_caches_to_executors(src_proj_dir: Path, dst_executors: List[Executor]):
    """
    소스 디렉토리에서 여러 실행기 프로젝트 디렉토리로 캐시 디렉토리 복사
    :param src_proj_dir: 복사할 소스 디렉토리
    :param dst_executors: 복사할 실행기 목록
    """
    print('Copying caches to other executors')
    dir_names_to_copy = ['.pio', 'build_cache']
    for dir_name_to_copy in dir_names_to_copy:
        src_path = Path(src_proj_dir, dir_name_to_copy)
        for dst_executor in dst_executors:
            dst_path = Path(dst_executor.proj_dir, dir_name_to_copy)
            shutil.copytree(src_path, dst_path)


def get_source_files_to_link() -> List[Path]:
    """
    로컬 프로젝트의 중요 파일 목록 생성. 추적되지 않은(하지만 필요한) 파일을 놓칠 수 있으므로
    여기서는 git을 사용하지 않았습니다.
    :return: 컴파일에 필요한 소스 파일 목록
    """
    local_proj_path = Path('.')
    venv_dirs = list(local_proj_path.glob('*venv*/'))
    # 빌드가 독립적이어야 하므로 .pio 디렉토리는 링크하지 않음
    pio_dirs = list(local_proj_path.glob('*.pio*/'))
    cmake_dirs = list(local_proj_path.glob('*cmake-build*/'))

    local_dirs_to_not_link = [Path('.git/'), Path('build_cache/')] + venv_dirs + pio_dirs + cmake_dirs
    local_filenames_to_not_link = [
        Path('Configuration_local.hpp'),
        Path('Configuration_local_matrix.hpp'),
    ]

    local_paths_to_link = []
    for local_dir_str, local_subdirs, local_files in os.walk(local_proj_path):
        local_dir_path = Path(local_dir_str)
        dir_shouldnt_be_linked = any(d == local_dir_path or d in local_dir_path.parents for d in local_dirs_to_not_link)
        if dir_shouldnt_be_linked:
            continue
        for local_file in local_files:
            local_file_full_path = Path(local_dir_path, local_file)
            file_shouldnt_be_linked = any(local_file_full_path == f for f in local_filenames_to_not_link)
            if file_shouldnt_be_linked:
                continue
            local_paths_to_link.append(local_file_full_path)
    return local_paths_to_link


def wait_for_executor_to_finish(executor_list: List[Executor], timeout=0.1, poll_time=0.2):
    """
    실행기가 빌드를 완료할 때까지 대기
    :param executor_list: 실행기 목록
    :param timeout: 실행 중인 프로세스와 통신할 시간(일종의 해킹)
    :param poll_time: 모든 실행기를 다시 확인하기 전 대기 시간
    """
    while get_finished_executor_idx(executor_list) is None:
        for e in executor_list:
            if e.proc is not None and e.proc.poll() is None:
                # 실행 중인 프로세스가 차단되지 않도록 통신
                # (즉, 출력이 너무 많음)
                try:
                    _ = e.proc.communicate(timeout=timeout)
                except subprocess.TimeoutExpired:
                    pass  # 예상되고 발생해야 하는 상황
        time.sleep(poll_time)
