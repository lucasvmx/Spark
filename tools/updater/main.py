#
# Spark updater
# Lucas Vieira <lucas.engen.cc@gmail.com>
#

from status_codes import ALREADY_UPDATED
from download import download_latest_version
from urllib.parse import urlparse
from urllib.request import urlopen
from os.path import split
from sys import exit
import argparse

def parse_version_string(version_str: str):
    """Analisa uma string contendo uma versão
    e retorna três inteiros contendo a versão separada

    Args:
        version_str (str): string contendo a versão

    Returns:
        int,int,int: major, minor e patch
    """

    ver = version_str.split(".")
    if len(ver) != 3:
        print("[ERROR] Invalid version detected: %s" % ver)
        exit(1)

    major = int(ver[0])
    minor = int(ver[1])
    patch = int(ver[2])

    return major, minor, patch

def get_latest_version():
    """Obtém a versão mais atual disponível do programa

    Args:
        release_type (str): Tipo da release

    Returns:
        str: String contendo a versão
    """

    try:
        url = "https://github.com/lucas-engen/Spark/releases/latest"
        response = urlopen(url)
        next_ver = response.url
        parse_result = urlparse(next_ver)
        ver = split(parse_result.path)[1][1:]
        splited_ver = ver.split(".")

        major = int(splited_ver[0])
        minor = int(splited_ver[1])
        patch = int(splited_ver[2])
        

        return major, minor, patch
    except Exception as err:
        return 0, 0, 0


if __name__ == "__main__":

    valid_archs = ["x86", "x64" ]

    ap = argparse.ArgumentParser("Spark Updater")
    ap.add_argument("--current_version", required=True, help="Current version string (ex: 1.5.2)")
    ap.add_argument("--arch", required=True, help="Target architeture (x86 or x64")
    args = vars(ap.parse_args())

    # Obtém a versão atual do programa
    major, minor, patch = parse_version_string(args["current_version"])

    # Obtém a versão mais recente do programa
    latest_major, latest_minor, latest_patch = get_latest_version()

    # Compara as versões
    curr_ver = major * 100 + minor * 10 + patch
    latest_ver = latest_major * 100 + latest_minor * 10 + latest_patch
    latest_ver_str = f"{latest_major}.{latest_minor}.{latest_patch}"

    print(f"{curr_ver} {latest_ver}")
    if curr_ver < latest_ver:
        print(f"[INFO] Update available: {latest_major}.{latest_minor}.{latest_patch}")
    else:
        print("[INFO] You have the latest version")
        exit(ALREADY_UPDATED)
    
    # Baixa a última versão do programa e executa
    download_latest_version(latest_ver_str, args["arch"])
