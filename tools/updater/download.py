#
# Spark updater
# Lucas Vieira <lucas.engen.cc@gmail.com>
#
from status_codes import UNPACK_FAILED
from urllib.request import urlretrieve
from pyunpack import Archive
from os import system, kill, getppid
from sys import stdout

progress_chars = ['/', '-', '\\', '|', '-']
g_downloaded: int = 0
g_index: int = 0

def get_next_char():
    global progress_chars
    global g_index

    if g_index == len(progress_chars):
        g_index = 0

    c = progress_chars[g_index]
    g_index = g_index + 1
    return c

def show_download_progress(block, size, total_size):
    global g_downloaded
    g_downloaded = g_downloaded + size
    stdout.write("Downloading ")
    stdout.write(f"{get_next_char()}\r")
    stdout.flush()

def download_latest_version(latest: str, arch: str):

    base_url = f"https://github.com/lucas-engen/Spark/releases/download/v{latest}/Spark-v{latest}-{arch}.exe"

    print(f"[INFO] Downloading Spark version {latest}")
    local_filename = f"Spark-v{latest}-{arch}.exe"
    filename, _ = urlretrieve(base_url, filename=local_filename, reporthook=show_download_progress)
    print(f"[INFO] Downloaded {filename}")
    print(f"[INFO] Running program ...")

    # Fecha o Spark
    from signal import SIGINT
    kill(getppid(), SIGINT)

    # Executa o arquivo baixado
    system(local_filename)
