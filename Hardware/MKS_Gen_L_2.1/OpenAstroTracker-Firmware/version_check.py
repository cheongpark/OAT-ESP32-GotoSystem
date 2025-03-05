#!/usr/bin/env python3

import sys
import argparse
import fnmatch
from typing import Optional, Dict

import mistune
import CppHeaderParser
import semver
import git

parser = argparse.ArgumentParser(description='''Check that the versioning of the repo has been correctly changed.
Intended to run in a PR CI''')
parser.add_argument('branchspec', default='origin/develop', nargs='?',
                    help='Git branch specification for previous file versions (default: %(default)s)')


def die(*args, error_code=1, **kwargs):
    print('ERROR: ', end='', file=sys.stderr)
    print(*args, **kwargs, file=sys.stderr)
    exit(error_code)


class SemVerWithVPrefix(semver.VersionInfo):
    @classmethod
    def parse(cls, version):
        if version[0] != 'V':
            raise ValueError(f"{version}: not a valid semantic version tag. Must start with 'v' or 'V'")
        self = super(SemVerWithVPrefix, cls).parse(version[1:])
        return self

    def __str__(self):
        return 'V' + super(SemVerWithVPrefix, self).__str__()


def only_changed_non_fw_files(refspec: str) -> bool:
    print('Checking if the files changed impact the firmware...')
    repo = git.Repo('.')
    index = repo.index
    origin_develop_commit = repo.commit(refspec)

    # 작업 복사본에 대한 인덱스 (스테이징되지 않은 파일들)
    changed_but_not_staged = [item.a_path for item in index.diff(None)]
    # 현재 HEAD 트리에 대한 인덱스 (스테이징된 파일들)
    changed_and_staged = [item.a_path for item in index.diff('HEAD')]
    # 현재 HEAD 트리에 대한 origin (브랜치에서 변경된 파일들)
    branch_files_changed = [item.a_path for item in origin_develop_commit.diff('HEAD')]

    print(f'Unstaged changes: {changed_but_not_staged}')
    print(f'Staged changes: {changed_and_staged}')
    print(f'Changed in branch: {branch_files_changed}')

    all_changed_filenames = changed_but_not_staged + changed_and_staged + branch_files_changed
    print(f'Total changed files: {all_changed_filenames}')

    non_fw_globs = [
        # 빌드와 관련없는 Python 스크립트 (pre/post Platformio 스크립트는 여전히 빌드 프로세스에 영향을 미침)
        'matrix_build*.py', 'version_check.py', 'scripts/MeadeCommandParser.py',
        # 텍스트 파일들
        '*.md', 'LICENSE', '.mailmap',
        # 빌드 시스템
        '.github/*.yml', 'requirements_*.txt',
        # 기타 도구 파일들
        '.gitignore', '.git-blame-ignore-revs',
    ]
    for changed_filename in all_changed_filenames:
        if not any(fnmatch.fnmatch(changed_filename, non_fw_glob) for non_fw_glob in non_fw_globs):
            # 변경된 파일이 non-FW 파일이 아닌 경우 (==> FW 파일인 경우)
            # 전체 버전 체크를 수행해야 함
            print(f"Changed file {changed_filename} will affect firmware ({non_fw_globs})")
            return False
    return True


def get_header_defines(header_contents: str) -> Dict[str, str]:
    defines_detail = CppHeaderParser.CppHeader(header_contents, argType='string').defines_detail
    return dict(d['value'].split(' ', maxsplit=1) for d in defines_detail)


def find_child_text(node: dict) -> Optional[str]:
    if 'children' in node.keys():
        if node['children'] and isinstance(node['children'], list):
            return find_child_text(node['children'][0])
    elif 'text' in node.keys():
        return node['text']
    return None


def get_header_version(version_header_contents: str) -> semver.VersionInfo:
    version_defines = get_header_defines(version_header_contents)
    try:
        version_str = next(v.strip('"') for k, v in version_defines.items() if 'version' in k.lower())
    except StopIteration:
        die(f'Could not find version define in\n{version_header_contents}: {version_defines}')
    try:
        version = SemVerWithVPrefix.parse(version_str)
    except ValueError as e:
        die(f'SemVer error: {e}')

    print(f'Version is {version}')
    return version


def get_changelog_version(changelog_markdown_path: str) -> str:
    markdown = mistune.create_markdown(renderer=mistune.AstRenderer())
    with open(changelog_markdown_path, 'r') as f:
        changelog_lines = f.read()
    changelog_ast = markdown(changelog_lines)

    first_heading_has_list = (changelog_ast[1]['type'] == 'list')
    if not first_heading_has_list:
        die(f'Latest changelog entry does not include a _list_ of what has changed')

    first_heading_str = find_child_text(changelog_ast[0])
    if not first_heading_str:
        die(f'Could not find the latest changelog heading: {changelog_ast[0]}')
    print(f'Latest changelog heading: {first_heading_str}')

    return first_heading_str


def get_previous_header_version(version_header_path: str, refspec: str) -> semver.VersionInfo:
    repo = git.Repo('.')
    previous_header_contents = repo.git.show(f'{refspec}:{version_header_path}')
    print(f'Checking previous header version from {refspec}:{version_header_path}')
    previous_version = get_header_version(previous_header_contents)
    print(f'Read previous header version: {previous_version}')
    return previous_version


def main():
    if only_changed_non_fw_files(args.branchspec):
        print('No FW files changed, not checking version.')
        return
    version_header_path = './Version.h'
    # 헤더에서 현재 버전 가져오기
    print(f'Checking current header version from {version_header_path}')
    with open(version_header_path, 'r') as fp:
        version_header_contents = fp.read()
        header_version = get_header_version(version_header_contents)

    # 이전 버전 가져오기 (git 사용)
    previous_header_version = get_previous_header_version(version_header_path, args.branchspec)

    # 버전이 증가했는지 확인
    if header_version <= previous_header_version:
        die(f'Header version must be greater than the previous version {header_version} <= {previous_header_version}')

    changelog_markdown_path = './Changelog.md'
    # changelog에서 문자열 가져오기, 최신 항목에 버전이 있어야 함
    changelog_version = get_changelog_version(changelog_markdown_path)

    # 헤더와 changelog가 일치하는지 확인
    if str(header_version).lower() not in changelog_version.lower():
        die(f'Header version ("{header_version}" in {version_header_path}) does \
not match changelog version ("{changelog_version}" in {changelog_markdown_path})')


if __name__ == '__main__':
    args = parser.parse_args()
    main()
    print('Finished version check!')
