#!/usr/bin/python3

import plumbum
import argparse
import tqdm

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', type=int)
    parser.add_argument('-m', type=int)
    parser.add_argument('--cnt', type=int)
    parser.add_argument('--connectivity', type=int)
    return parser.parse_args()


def run(n, m, seed, connectivity):
    test_name = f'random_{connectivity}_{n}_{m}_{seed}'

    cmd = plumbum.local['./build/random_graph_generator']['-n', n]['-m', m]['--seed', seed]['--connectivity', connectivity]['--dest', f'gen/{test_name}.txt']['--dot', f'gen/{test_name}.dot']
    print(f'running: {cmd}')
    cmd & plumbum.FG

def main():
    args = parse_args()

    for seed in range(1, args.cnt + 1):
        run(args.n, args.m, seed, args.connectivity)


if __name__ == '__main__':
    main()
