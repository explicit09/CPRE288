#!/usr/bin/env python3
"""Mac-only Lab8 build/program commands; outputs stay under .vscode/."""
import argparse
import pathlib
import shutil
import subprocess
import sys

REPO = pathlib.Path(__file__).resolve().parent.parent
LOCAL = REPO / '.vscode'
SDK = pathlib.Path('/Users/tadiwa/Documents/Codex/2026-10-21/can-t-complete-intellisense-yet-because/outputs/TivaWare_C_Series-2.2.0.295')
CGT = pathlib.Path('/Users/tadiwa/Documents/Codex/2026-10-21/can-t-complete-intellisense-yet-because/outputs/ti-cgt-arm/ti-cgt-arm_20.2.7.LTS')
OPENOCD = '/opt/homebrew/bin/openocd'
BOARD = 'board/ti_ek-tm4c123gxl.cfg'

def run(args, cwd=None):
    print('+ ' + ' '.join(map(str, args)), flush=True)
    subprocess.run(list(map(str, args)), cwd=cwd, check=True)

def build():
    project = REPO / 'Lab8'
    out = LOCAL / 'tiva-build' / 'Lab8'
    out.mkdir(parents=True, exist_ok=True)
    image = out / 'Lab8.out'
    image.unlink(missing_ok=True)
    compiler = CGT / 'bin' / 'armcl'
    if not compiler.is_file():
        raise RuntimeError('TI ARM compiler is missing: ' + str(compiler))
    flags = ['-mv7M4', '--code_state=16', '--float_support=FPv4SPD16', '-me', '--abi=eabi',
             '--define=ccs="ccs"', '--define=PART_TM4C123GH6PM', '-g', '--gcc',
             '--diag_warning=225', '--diag_wrap=off', '--display_error_number']
    includes = ['--include_path=' + str(p) for p in (project, SDK, CGT / 'include')]
    sources = sorted(project.glob('*.c'))
    if not sources:
        raise RuntimeError('No Lab8 C sources found')
    objects = []
    for source in sources:
        obj = out / (source.stem + '.obj')
        run([compiler, *flags, *includes, '--compile_only', '--output_file=' + str(obj), source], out)
        objects.append(obj)
    run([compiler, *flags, '-z', '--map_file=' + str(out / 'Lab8.map'),
         '--heap_size=1024', '--stack_size=2048', '--search_path=' + str(CGT / 'lib'),
         '--search_path=' + str(CGT / 'include'), '--reread_libs', '--diag_wrap=off',
         '--display_error_number', '--warn_sections', '--xml_link_info=' + str(out / 'Lab8_linkInfo.xml'),
         '--rom_model', '--output_file=' + str(image), *objects, project / 'libcybotScan.lib',
         project / 'tm4c123gh6pm.cmd', '--library=libc.a',
         '--library=' + str(SDK / 'driverlib/ccs/Debug/driverlib.lib')], out)
    print('Built firmware: ' + str(image), flush=True)
    return image

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument('action', choices=['build', 'probe', 'flash'])
    args = parser.parse_args()
    if args.action == 'build':
        build()
    elif args.action == 'probe':
        run([OPENOCD, '-f', BOARD, '-c', 'init; shutdown'])
    else:
        # Never flash an old image after a failed build.
        image = build()
        print('Programming Lab8 firmware via ICDI; verification and reset follow.', flush=True)
        run([OPENOCD, '-f', BOARD, '-c', 'program {' + str(image) + '} verify reset exit'])

if __name__ == '__main__':
    try:
        main()
    except (subprocess.CalledProcessError, RuntimeError, OSError) as error:
        print('Tiva setup: ' + str(error), file=sys.stderr)
        sys.exit(1)
