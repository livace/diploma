#!/usr/bin/env python3
import argparse
import logging
import plumbum
import sys
import hashlib
import filelock

from common import setup_logging
from root import repo_root


class BuildConfig:
    def __init__(self):
        self.build_type = 'Release'
        self.build_dir = 'build'
        self.rebuild = False
        self.sanitize = None
        self.log_level = None
        self.minimal_log_level = None
        self.target = None
        self.defines = []

    def to_json(self):
        return {
            'build_type': self.build_type,
            'build_dir': self.build_dir,
            'force_rebuild': self.rebuild,
            'sanitize': self.sanitize,
            'log_level': self.log_level,
            'minimal_log_level': self.minimal_log_level,
            'target': self.target,
            'defines': self.defines
        }

    def hash(self):
        return hashlib.sha256(str([
            self.build_type,
            self.log_level,
            self.sanitize,
            self.defines
        ]).encode()).hexdigest()

    def get_build_type(args):
        if args.debug:
            return 'Debug'
        elif args.release:
            return 'Release'
        return 'Release'    # looks like reasonable default

    def parse_args():
        parser = argparse.ArgumentParser()
        parser.add_argument('--debug', action='store_true', help='set CMAKE_BUILD_TYPE to Debug')
        parser.add_argument('--release', action='store_true', help='set CMAKE_BUILD_TYPE to Release')
        parser.add_argument('--sanitize', help='build with sanitizers')
        parser.add_argument('--log-level', help='default log level')
        parser.add_argument('--minimal-log-level', help='minimal log level')
        parser.add_argument('--build-dir', default='build', help='build directory')
        parser.add_argument('--rebuild', action='store_true', help='delete build directory before building')
        parser.add_argument('--define', dest='defines', nargs='*', help='build defines')
        parser.add_argument('target', nargs='?', help='target to build')
        args = parser.parse_args()

        assert not (args.debug and args.release)

        config = BuildConfig()
        config.build_type = BuildConfig.get_build_type(args)
        if args.build_dir is not None:
            config.build_dir = args.build_dir
        if args.rebuild:
            config.rebuild = True
        config.sanitize = args.sanitize
        config.log_level = args.log_level
        config.minimal_log_level = args.minimal_log_level
        config.target = args.target
        config.defines = args.defines

        return config


def run_cmake(build_config):
    cmd = plumbum.local['cmake']['..']['-GNinja']

    if build_config.build_type is not None:
        cmd = cmd[f'-DCMAKE_BUILD_TYPE={build_config.build_type}']

    if build_config.log_level is not None:
        cmd = cmd[f'-DLOG_LEVEL={build_config.log_level.capitalize()}']

    if build_config.minimal_log_level is not None:
        cmd = cmd[f'-DMINIMAL_LOG_LEVEL={build_config.minimal_log_level.capitalize()}']

    if build_config.sanitize is not None:
        cmd = cmd[f'-DSANITIZE={build_config.sanitize}']

    if build_config.defines is not None:
        for define in build_config.defines:
            if define.find('=') != -1:
                cmd = cmd[f'-DDEFINE_{define}']
            else:
                cmd = cmd[f'-DDEFINE_{define}=1']

    logging.info(f'running cmake: {cmd}')
    try:
        cmd & plumbum.FG
        return True
    except:
        logging.error('cmake failed')
        return False


def run_build(build_config):
    cmd = plumbum.local['ninja']
    if build_config.target is not None:
        cmd = cmd[build_config.target]

    logging.info(f'running ninja: {cmd}')
    try:
        cmd & plumbum.FG
        return True
    except Exception as e:
        logging.error(f'ninja failed: {e}')
        return False


def build_unsafe(build_config):
    c_compiler = 'clang-14'
    cpp_compiler = 'clang++-14'

    build_dir = f'build_{build_config.hash()}'

    if build_config.rebuild:
        try:
            plumbum.local['rm']['-rf'][build_dir] & plumbum.FG
        except:
            logging.warning('build directory does not exist, creating it')

    plumbum.local['mkdir']['-p'][build_dir] & plumbum.FG
    plumbum.local['rm']['-rf'][build_config.build_dir] & plumbum.FG
    plumbum.local['ln']['-sfn'][build_dir][build_config.build_dir] & plumbum.FG

    def cmake():
        return run_cmake(build_config)

    with plumbum.local.env(CMAKE_CXX_COMPILER=cpp_compiler, CXX=cpp_compiler):
        with plumbum.local.env(CMAKE_C_COMPILER=c_compiler, CC=c_compiler):
            with plumbum.local.cwd(build_dir):
                cmake_result = cmake()
            if not cmake_result:
                logging.warning('cmake failed, trying to delete build directory and rerun')
                plumbum.local['rm']['-rf'][build_dir] & plumbum.FG
                plumbum.local['mkdir']['-p'][build_dir] & plumbum.FG

                with plumbum.local.cwd(build_dir):
                    cmake_result = cmake()
                if not cmake_result:
                    logging.error('cmake failed again, exiting:')
                    return 1

            with plumbum.local.cwd(build_dir):
               build_result = run_build(build_config)
            if not build_result:
                return 1

    return 0


def build(config):
    logging.getLogger("filelock").setLevel(logging.ERROR)
    with filelock.FileLock('build.lock'):
        return build_unsafe(config)

def main():
    setup_logging()
    build_config = BuildConfig.parse_args()
    with plumbum.local.cwd(repo_root):
        return build(build_config)


if __name__ == '__main__':
    sys.exit(main())
