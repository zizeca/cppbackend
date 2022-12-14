import argparse
import subprocess
import time
import random
import shlex

RANDOM_LIMIT = 1000
SEED = 123456789
random.seed(SEED)

AMMUNITION = [
    'localhost:8080/api/v1/maps/map1',
    'localhost:8080/api/v1/maps'
]

SHOOT_COUNT = 100
COOLDOWN = 0.1


def start_server():
    parser = argparse.ArgumentParser()
    parser.add_argument('server', type=str)
    return parser.parse_args().server


def run(command, output=None, inp=None):
    process = subprocess.Popen(shlex.split(command), stdin=inp, stdout=output, stderr=subprocess.DEVNULL)
    return process


def stop(process, wait=False):
    if process.poll() is None and wait:
        process.wait()
    process.terminate()


def shoot(ammo):
    hit = run('curl ' + ammo, output=subprocess.DEVNULL)
    time.sleep(COOLDOWN)
    stop(hit, wait=True)


def make_shots():
    for _ in range(SHOOT_COUNT):
        ammo_number = random.randrange(RANDOM_LIMIT) % len(AMMUNITION)
        shoot(AMMUNITION[ammo_number])
    print('Shooting complete')


server = run(start_server())
pf = run("sudo perf record -o perf.data -p " + str(server.pid))
make_shots()
time.sleep(1)
stop(pf)
time.sleep(1)
stop(server)
p1 = run("sudo perf script", output=subprocess.PIPE)
p2 = run("./FlameGraph/stackcollapse-perf.pl", output=subprocess.PIPE, inp=p1.stdout)
p1.wait()
fout = open('graph.svg', 'wt')
p3 = run("./FlameGraph/flamegraph.pl", output=fout, inp=p2.stdout)

#stop(p1, wait=True)
#stop(p2, wait=True)
#stop(p3, wait=True)
time.sleep(1)
print('Job done')
# python3 shoot.py "/test_cpp_backend/sprint1/problems/map_json/solution/build/bin/game_server /test_cpp_backend/sprint1/problems/map_json/solution/data/config.json" || true
