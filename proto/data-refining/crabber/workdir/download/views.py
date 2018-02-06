import os

from django.shortcuts import render
from django.http import HttpResponse

from app.settings import DATA_DIR
# from .download import download_modis_terra_l2
from .models import Task


def download(request, year, day, suffix):
    # Declaring variables
    response = HttpResponse()

    response.write('<h1>Crabber - crawl and download</h1>')

    """
    # Check and create 'year' directory
    if not os.path.exists(DATA_DIR + year):
        os.makedirs(DATA_DIR + year)
    # Check and create 'day' directory
    if not os.path.exists(DATA_DIR + year + '/' + day):
        os.makedirs(DATA_DIR + year + '/' + day)

    # Downloading MODIS Terra L2 data
    if suffix.lower() == 'modis-terra-l2':
        response.write('<div>Downloading ' + str(suffix) + ':</div>')
        response.write(
            download_modis_terra_l2(year, day)
        )
        response.write("<br /><br />>Download task quered<<br />")
    else:
        response.write("You need to cast right parametrs for download <br />")

    """
    return response
