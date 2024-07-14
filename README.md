# pkOSmol

pkOSmol is an exercise in OS development targeted at embedded systems.

## Building

1. Clone the repository and download Renode:
  ```sh
  git clone https://github.com/pkoscik/pkOSmol.git && cd pkOSmol
  ./tools/download_renode.sh
  ```

2. Build the sample
  ```sh
  ./tools/build.py hifive1_revb hello_world
  ```

3. Simulate the payload in Renode
  ```sh
  ./tools/build.py -r hifive1_revb
  ```
