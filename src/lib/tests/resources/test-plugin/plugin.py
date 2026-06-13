#!/usr/bin/env python3
import sys, json

for line in sys.stdin:
    line = line.strip()
    if not line:
        continue

    req = json.loads(line)
    if req.get('op') == 'load':
        key = req.get('key', '')
        fields = req.get('fields', [])
        if key == 'error_key':
            resp = {'error': 'test error'}
        elif fields:
            resp = {'value': {f: 'value_for_' + f for f in fields}}
        elif key == 'date_key':
            resp = {'value': '2026-06-13T10:30:00'}
        else:
            resp = {'value': 'test-value'}
    elif req.get('op') == 'action':
        t = req.get('type', '')
        if t == 'error_action':
            resp = {'error': 'action failed'}
        elif t == 'move_action':
            resp = {'path': '/new/path/file.jpg'}
        else:
            resp = {}
    else:
        resp = {'error': 'unknown op'}

    sys.stdout.write(json.dumps(resp) + '\n')
    sys.stdout.flush()
