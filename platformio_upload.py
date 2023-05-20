# Allows PlatformIO to upload directly to AsyncElegantOTA
#
# To use:
# - copy this script into the same folder as your platformio.ini
# - set the following for your project in platformio.ini:
#
# extra_scripts = platformio_upload.py
# upload_protocol = custom
# custom_ota_upload_url = <your upload URL>
#
# An example of an upload URL:
# upload_URL = http://192.168.1.123/update

# pylint: disable-all
# mypy: ignore-errors
import hashlib

import requests

Import("env")

try:
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm
except ImportError:
    env.Execute("$PYTHONEXE -m pip install requests_toolbelt")
    env.Execute("$PYTHONEXE -m pip install tqdm")
    from requests_toolbelt import MultipartEncoder, MultipartEncoderMonitor
    from tqdm import tqdm


def on_upload(source, target, env):
    firmware_path = str(source[0])
    custom_ota_upload_url = env.GetProjectOption("custom_ota_upload_url")

    with open(firmware_path, "rb") as firmware:
        md5 = hashlib.md5(firmware.read()).hexdigest()
        firmware.seek(0)
        encoder = MultipartEncoder(
            fields={
                "MD5": md5,
                "firmware": ("firmware", firmware, "application/octet-stream"),
            }
        )

        bar = tqdm(
            desc="Upload Progress",
            total=encoder.len,
            dynamic_ncols=True,
            unit="B",
            unit_scale=True,
            unit_divisor=1024,
        )

        monitor = MultipartEncoderMonitor(
            encoder, lambda monitor: bar.update(monitor.bytes_read - bar.n)
        )

        response = requests.post(
            custom_ota_upload_url,
            data=monitor,
            headers={"Content-Type": monitor.content_type},
        )
        bar.close()
        print(response, response.text)


env.Replace(UPLOADCMD=on_upload)
