#!/usr/bin/python3

import plumbum
import argparse
import tqdm
import pandas as pd
import multiprocessing
from functools import reduce

def parse_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('-n', required=True, type=int)
    parser.add_argument('-m', required=True, type=int)
    parser.add_argument('--cnt', required=True, type=int)
    parser.add_argument('--connectivity', required=True, type=int)
    parser.add_argument('--number', required=True, type=int)
    parser.add_argument('--result', required=True)
    parser.add_argument('--depth', type=float, required=True)
    parser.add_argument('--policy', required=True)
    parser.add_argument('--vertex-limit', type=int, required=True)
    parser.add_argument('--greedy-size', required=True)

    return parser.parse_args()



def generate_test(n, m, seed, connectivity):
    test_name = f'random_{connectivity}_{n}_{m}_{seed}'

    cmd = plumbum.local[
        './build/random_graph_generator'
    ][
        '-n', n
    ][
        '-m', m
    ][
        '--seed', seed
    ][
        '--connectivity', connectivity
    ][
        '--dest', f'gen/{test_name}.txt'
    ][
        '--dot', f'gen/{test_name}.dot'
    ]
    print(f'running: {cmd}')
    cmd & plumbum.FG

def run(bin, n, m, seed, connectivity, depth, policy, reset_depth, vertex_limit, number, greedy_size):
    generate_test(n, m, seed, connectivity)

    test_name = f'random_{connectivity}_{n}_{m}_{seed}'

    results = []

    # for i in range(n):
    for i in range(1):
        file = f'_score/{test_name}_{i}_dfs.txt'
        cmd = plumbum.local[
            bin
        ][
            '-p', f'gen/{test_name}.txt'
        ][
            '-v', i
        ][
            '--score', file
        ][
            '--depth', depth
        ][
            '--policy', policy
        ][
            '--reset-depth', reset_depth
        ][
            '--vertex-limit', vertex_limit
        ][
            '--greedy-size', greedy_size
        ]

        if number >= 2:
            cmd = cmd['--trees'][number]
        print(f'running: {cmd}')
        retcode, _, _ = cmd.run()

        res = {}
        with open(file) as f:
            data = f.read().split(' ')
            score, len_1, len_2 = map(float, data[:3])
            cnt = int(data[3])

            for i in range(cnt):
                cnt_breaks = int(data[4 + i * 3])
                fail_prob, mean_disconnected = map(float, data[5 + i * 3:7 + i * 3])

                res[f'fail_prob_{cnt_breaks}'] = fail_prob
                res[f'mean_disconnected_{cnt_breaks}'] = mean_disconnected

        results.append({
            'n': n,
            'm': m,
            'connectivity': connectivity,
            'seed': seed,
            'method': 'dfs',
            'score': score,
            'len_1': len_1,
            'len_2': len_2,
            'bin': bin,
            'vertex': i,
            'depth': depth,
            'policy': policy,
            'reset_depth': reset_depth,
            'vertex_limit': vertex_limit,
            'number': number,
            'greedy_size': greedy_size,
            **res
        })

    return results


def main():
    args = parse_args()

    wins = 0
    loses = 0
    fails = 0

    if args.number == 1:
        bin = './build/junk_algos_one'
    # if args.number == 2:
        # bin = './build/junk_algos_dfs'
    else:
        bin = './build/junk_algos_more_dfs'

    result = []

    run_args = []

    for seed in range(1, args.cnt + 1):
        run_args.append((
            bin,
            args.n,
            args.m,
            seed,
            args.connectivity,
            args.depth,
            args.policy,
            True,  # reset_depth
            args.vertex_limit,
            args.number,
            args.greedy_size
        ))

    result = reduce(lambda a, b: a + b, multiprocessing.Pool(50).starmap(run, run_args), [])

    pd.DataFrame(result).to_csv(args.result, index=None)

if __name__ == '__main__':
    main()
