#!/usr/bin/env python3
import socket
import argparse
import matplotlib.pyplot as plt

ap = argparse.ArgumentParser()
ap.add_argument("ip", nargs="?", default="0.0.0.0")
ap.add_argument("-p", "--port", type=int, default=5555)
args = ap.parse_args()

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((args.ip, args.port))
sock.setblocking(False)

xs, ys = [], []

plt.ion()
fig, ax = plt.subplots()
ax.set_title("Lennard-Jones (live)")
ax.set_xlabel("r / sigma")
ax.set_ylabel("U(r)")
ax.grid(True)

(line,) = ax.plot([], [], "-", lw=1.5)

print(f"listening UDP on {args.ip}:{args.port}")

while plt.fignum_exists(fig.number):
    try:
        data, _ = sock.recvfrom(4096)
    except BlockingIOError:
        data = None

    if data:
        for s in data.decode(errors="ignore").splitlines():
            p = s.split()
            if len(p) >= 3 and p[0] == "LJ":
                try:
                    xs.append(float(p[1]))
                    ys.append(float(p[2]))
                except ValueError:
                    pass

        # tri pour avoir une courbe propre
        pts = sorted(zip(xs, ys))
        xs2, ys2 = zip(*pts)
        line.set_data(xs2, ys2)
        ax.relim()
        ax.autoscale_view()

    plt.pause(0.02)
