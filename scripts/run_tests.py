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
    parser.add_argument('--method', required=True)
    parser.add_argument('--number', required=True, type=int)
    parser.add_argument('--result', required=True)
    return parser.parse_args()



def generate_test(n, m, seed, connectivity):
    test_name = f'random_{connectivity}_{n}_{m}_{seed}'

    cmd = plumbum.local['./build/random_graph_generator']['-n', n]['-m', m]['--seed', seed]['--connectivity', connectivity]['--dest', f'gen/{test_name}.txt']['--dot', f'gen/{test_name}.dot']
    print(f'running: {cmd}')
    cmd & plumbum.FG

def run(bin, n, m, seed, connectivity, method):
    generate_test(n, m, seed, connectivity)

    test_name = f'random_{connectivity}_{n}_{m}_{seed}'

    results = []

    for i in range(n):
        file = f'score_{test_name}_{i}_{method}.txt'
        cmd = plumbum.local[bin]['-p', f'gen/{test_name}.txt']['-v', i]['-m', method]['--score', file]
        print(f'running: {cmd}')
        retcode, _, _ = cmd.run()

        with open(file) as f:
            score = int(f.read())

        results.append({
            'n': n,
            'm': m,
            'connectivity': connectivity,
            'seed': seed,
            'method': method,
            'score': score,
            'bin': bin,
            'vertex': i
        })

    return results


def main():
    args = parse_args()

    wins = 0
    loses = 0
    fails = 0

    if args.number == 2:
        bin = './build/junk_algos'
    if args.number == 3:
        bin = './build/junk_algos_three'

    result = []

    run_args = []

    for seed in range(1, args.cnt + 1):
        run_args.append((
            bin,
            args.n,
            args.m,
            seed,
            args.connectivity,
            args.method
        ))

    result = reduce(lambda a, b: a + b, multiprocessing.Pool(50).starmap(run, run_args), [])
    # result += run(bin=bin, n=args.n, m=args.m, seed=seed, connectivity=args.connectivity, method=args.method)

    pd.DataFrame(result).to_csv(args.result, index=None)

if __name__ == '__main__':
    main()
