Import("env", "projenv")

import os
import tempfile
from pathlib import Path


def cprint(*args, **kwargs):
    print(f'post_script_remove_patched_files.py:', *args, **kwargs)


def clean_up_patched_files(*_, **__):
    """
    빌드 프로세스에서 이전에 생성된 모든 임시 패치 파일을 제거합니다
    """
    # patch_path_key는 pre_script_patch_debug.py와 동기화되어야 합니다
    # 현재 디렉토리 이름을 키에 넣어서 현재 빌드 프로세스에서 생성된 
    # 패치 파일만 제거하도록 합니다.
    # 이는 서로 다른 디렉토리에서 동시에 여러 빌드가 실행될 때 
    # 안전장치로만 유용합니다. (즉, 다른 프로세스의 파일이 사용 중일 때 
    # 해당 파일을 제거하지 않도록 합니다)
    project_dir_name = Path.cwd().name
    patch_path_key = f'_{project_dir_name}_patched_'
    tempdir_path = tempfile.gettempdir()
    cprint(f'Temp file dir is {tempdir_path}')
    patched_filepaths = []
    for filename in os.listdir(tempdir_path):
        full_filepath = os.path.join(tempdir_path, filename)
        if os.path.isfile(full_filepath) and patch_path_key in filename:
            patched_filepaths.append(full_filepath)
    for patched_filepath in patched_filepaths:
        cprint(f'Removing {patched_filepath}')
        try:
            os.remove(patched_filepath)
            pass
        except FileNotFoundError:
            cprint('Not found (deleted already?)')


env.AddPostAction('buildprog', clean_up_patched_files)
