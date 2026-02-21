# Broken msdscript Implementations Report

Environment notes:
- Host architecture: `arm64`
- Provided binaries `msdscript0..7` are `x86_64` Mach-O executables.
- Tested via wrapper scripts `msdscript/msdscript*_x86.sh` that run binaries using `arch -x86_64`.

## Single-program random testing

### msdscript0
- Command: `./test_msdscript ./msdscript0_x86.sh`
- Failing input: `12`
- Observed failure:
  - Exit code: `1`
  - stderr: `bad flag:`
  - `--interp`, `--print`, and `--pretty-print` all fail

### msdscript1
- Command: `./test_msdscript ./msdscript1_x86.sh`
- Failing input: `41`
- Observed failure:
  - Exit code: `1`
  - stderr: `bad flag:`
  - `--interp`, `--print`, and `--pretty-print` all fail

## Differential random testing against reference `./msdscript`

### msdscript2
- Command: `./test_msdscript ./msdscript ./msdscript2_x86.sh`
- Mode: `--interp`
- Failing input:
```txt
(_let vytqs = (_let vbtt = (-36 * 14) _in 19) _in (_let vcho = ((-3 + 5) * ((_let vdqi = vytqs _in 43) * (_let veigz = (vytqs * 22) _in veigz))) _in (_let vfbjk = -43 _in (vcho + vcho))))
```
- Reference (`./msdscript`): exit `0`, out `71896`, err empty
- Compared impl (`./msdscript2_x86.sh`): exit `1`, out empty, err `bad flag:`

### msdscript3
- Command: `./test_msdscript ./msdscript ./msdscript3_x86.sh`
- Mode: `--interp`
- Failing input:
```txt
(_let vjhcq = (_let vbsa = (_let vcyr = (-6 * 34) _in vcyr) _in -49) _in vjhcq)
```
- Reference (`./msdscript`): exit `0`, out `-49`, err empty
- Compared impl (`./msdscript3_x86.sh`): exit `1`, out empty, err `bad flag:`

### msdscript4
- Command: `./test_msdscript ./msdscript ./msdscript4_x86.sh`
- Mode: `--interp`
- Failing input:
```txt
(-1 * (_let vbv = (_let vbquz = 33 _in -7) _in (vbv * 28)))
```
- Reference (`./msdscript`): exit `0`, out `196`, err empty
- Compared impl (`./msdscript4_x86.sh`): exit `1`, out empty, err `bad flag:`

### msdscript5
- Command: `./test_msdscript ./msdscript ./msdscript5_x86.sh`
- Mode: `--interp`
- Failing input:
```txt
(_let vthoz = (_let vbwbd = ((_let vccvob = -40 _in vccvob) + 42) _in vbwbd) _in vthoz)
```
- Reference (`./msdscript`): exit `0`, out `2`, err empty
- Compared impl (`./msdscript5_x86.sh`): exit `1`, out empty, err `bad flag:`

### msdscript6
- Command: `./test_msdscript ./msdscript ./msdscript6_x86.sh`
- Mode: `--interp`
- Failing input:
```txt
(((38 * (_let vwi = (12 + (_let vbfm = 24 _in 41)) _in (_let vcpknx = (14 * 1) _in (-13 * -46)))) + -18) + ((_let vdyma = 49 _in (((-24 * vdyma) + 28) * (_let veflso = (_let vflw = vdyma _in 2) _in (veflso + veflso)))) + (_let vgvxs = (_let vhvdfw = -12 _in ((_let vigw = 19 _in vigw) + (_let vjkjg = vhvdfw _in vjkjg))) _in 22)))
```
- Reference (`./msdscript`): exit `0`, out `18136`, err empty
- Compared impl (`./msdscript6_x86.sh`): exit `1`, out empty, err `bad flag:`

### msdscript7
- Command: `./test_msdscript ./msdscript ./msdscript7_x86.sh`
- Mode: `--interp`
- Failing input: `18`
- Reference (`./msdscript`): exit `0`, out `18`, err empty
- Compared impl (`./msdscript7_x86.sh`): exit `1`, out empty, err `bad flag:`
