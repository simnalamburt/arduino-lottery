Arduino Lottery
========
Use [PlatformIO](http://platformio.org/) to build project.
```bash
python3 -m venv .venv && . .venv/bin/activate && pip install -U pip setuptools
pip install -r requirements.txt

# Build project
platformio run

# Upload to connected Arduono machine
platformio run -t upload

# Monitor serialports
platformio serialports monitor
```
