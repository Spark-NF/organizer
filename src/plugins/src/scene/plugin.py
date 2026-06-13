#!/usr/bin/env python3
import sys
import json
import os

try:
    from guessit import guessit
except ImportError:
    guessit = None


def normalize(val):
    if isinstance(val, list):
        val = val[0]
    return val if isinstance(val, (int, float, bool, str)) else str(val)


def run(req):
    op = req.get("op")

    if op == "load":
        key = req.get("key", "")
        path = req.get("path", "")

        if key == "scene":
            if guessit is None:
                return {"error": "guessit is not installed (pip install guessit)"}

            match = guessit(os.path.basename(path))
            return {"value": {field: normalize(val) for field, val in match.items()}}

        return {"error": f"unknown loader key: {key}"}

    return {"error": f"unknown op: {op}"}


for line in sys.stdin:
    # Read the input
    line = line.strip()
    if not line:
        continue

    # Run the actual logic
    resp = run(json.loads(line))

    # Write the response
    sys.stdout.write(json.dumps(resp) + "\n")
    sys.stdout.flush()
