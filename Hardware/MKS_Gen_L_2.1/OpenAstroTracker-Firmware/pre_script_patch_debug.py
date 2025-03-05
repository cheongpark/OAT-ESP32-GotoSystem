Import("env")

import os
import tempfile
from pathlib import Path


def cprint(*args, **kwargs):
    print(f'pre_script_patch_debug.py:', *args, **kwargs)


def get_winterrupts_path():
    winterrupts_path = None
    for pio_package in env.PioPlatform().dump_used_packages():
        pio_dir = env.PioPlatform().get_package_dir(pio_package['name'])
        # TODO: 비-메가 코어에 대해서는 이것이 변경되어야 합니다!
        possible_path = os.path.join(pio_dir, 'cores', 'MegaCore', 'WInterrupts.c')
        if os.path.exists(possible_path):
            cprint(f'Found WInterrupts.c: {possible_path}')
            winterrupts_path = possible_path
    return winterrupts_path


def patch_function_factory(src_path, output_suffix, replacement_list):
    """
    패치된 소스 파일의 파일 경로를 반환하는 함수를 생성합니다
    :param src_path: 디스크의 실제 소스 경로, node.get_abspath()와는 다릅니다
    :param output_suffix: 출력 임시 파일의 접미사
    :param replacement_list: 교체되어야 할 'in'/'out' 쌍의 목록
    :return: 빌드 미들웨어 함수
    """
    def out_func(node):
        # patch_path_key는 post_script_remove_patched_files.py와 동기화되어야 합니다
        # 성공적인 빌드 후 패치된 파일을 제거할 수 있도록 하기 위함입니다
        project_dir_name = Path.cwd().name  # 이것이 필요한 이유는 post_script_remove_patched_files.py를 참조하세요
        patch_path_key = f'_{project_dir_name}_patched_'
        with tempfile.NamedTemporaryFile(mode='w', suffix=f'{patch_path_key}{output_suffix}', delete=False) as tf:
            patched_filepath = tf.name
            cprint(f'Patching {src_path}')
            cprint(f'Replacement path: {patched_filepath}')
            cprint(f'Build path: {node.get_abspath()}')
            with open(src_path, 'r') as wint_f:
                for wint_line in wint_f.readlines():
                    # 기본값은 교체되지 않은 라인을 그대로 통과시키는 것입니다
                    out_line = wint_line
                    # 이제 라인이 교체 목록에 있는지 확인합니다
                    for replacement in replacement_list:
                        if replacement['in'] in wint_line:
                            out_line = replacement['out']
                            break
                    # (가능한 교체된) 라인을 출력 임시 파일에 씁니다
                    tf.write(out_line)
        return env.File(patched_filepath)
    return out_func


source_patch_dict = {
    '*WInterrupts.c': {
        'actual_src_path': get_winterrupts_path(),
        'patches': [
            {
                'in': 'IMPLEMENT_ISR(INT7_vect, EXTERNAL_INT_7)',
                'out': '''\
#if defined(OAT_DEBUG_BUILD)
  #pragma message "OAT_DEBUG_BUILD is defined, ISR 7 disabled in WInterrupts.c"
#else
  IMPLEMENT_ISR(INT7_vect, EXTERNAL_INT_7)
#endif
'''
            },
        ]
    }
}

for filepath_glob, file_patch_info in source_patch_dict.items():
    file_src_path = file_patch_info['actual_src_path']
    if not file_src_path:
        cprint(f'Could not find {filepath_glob} to patch! Skipping...')
        continue

    env.AddBuildMiddleware(
        patch_function_factory(src_path=file_src_path,
                               replacement_list=file_patch_info['patches'],
                               output_suffix='WInterrupts.c'),
        filepath_glob
    )
