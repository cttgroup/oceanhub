# ocean
from django.db import models
from django.contrib.gis.db import models
from django.contrib.gis.geos import Point
from django.utils import timezone
from django.contrib.gis.geos import GEOSGeometry
# from django.contrib.gis.geos import LineString
from django.contrib.gis.geos import Polygon, MultiPolygon
from django.conf import settings
from django.db.models.signals import post_save
from django.dispatch import receiver
from rest_framework.authtoken.models import Token


class Characteristic(models.Model):
    title = models.CharField(
        'Название', max_length=50, default='')
    description = models.CharField(
        'Описание', max_length=200, blank=True)
    model = models.SlugField(
        'Модель данных', default='', unique=True)
    tag = models.SlugField(
        'Опция для парсера (например, тэг в netCDF/hdf файле)', default='')
    store_prefix = models.SlugField(
        'Префикс хранения файлов', default='')
    file_mask = models.CharField(
        'Маска поиска файла', default='', max_length=50, blank=True)

    def __str__(self):
        # out = self.title if self.tag == '' else self.tag
        return self.title


class Region(models.Model):
    def __str__(self):
        return self.title

    """
    # Norv Sea
    poly = Polygon((
        (5.897592, 59.453594),
        (-15.743695, 64.412255),
        (15.617894, 77.781198),
        (27.955703, 70.515584),
        (5.897592, 59.453594),
    ))
    # Norv Sea trapezoid
    polygon = Polygon((
        (-15.743695, 77.781198),
        (27.955703, 77.781198),
        (27.955703, 59.453594),
        (-15.743695, 59.453594),
        (-15.743695, 77.781198),
    ))
    """

    # Norv Sea
    norvSea = Polygon((
        (5.897592, 59.453594),
        (-15.743695, 64.412255),
        (15.617894, 77.781198),
        (27.955703, 70.515584),
        (5.897592, 59.453594),
    ))
    # Kurils islands
    # lon1 = 145.933452, lat1 = 43,902974
    # lon2 = 151.670932, lat2 = 39,980028
    kurils = Polygon((
        (145.933452, 43,902974),
        (151.670932, 43,902974),
        (151.670932, 39,980028),
        (145.933452, 39,980028),
        (145.933452, 43,902974),
    ))
    mpolygon = MultiPolygon(norvSea, kurils)

    title = models.CharField(max_length=50, default='Default regions of interest')
    slug = models.SlugField(default='default')
    # poly = models.PolygonField(default=polygon)
    mpoly = models.MultiPolygonField(default=mpolygon)


# Bathymetric model
class Bathymetric(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Bathymetric height - sea floor height, meters
    value = models.FloatField("Высота поверхности океана, м", null=True)


# Sea surface temperature model
class SeaSurfaceTemperature(models.Model):
    # Longitude and latitude geo point
    # point = models.PointField(default=Point(6, 70), unique=True)
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    # datetime = models.DateTimeField(default=timezone.now, unique_for_date=True)
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Sea surface temperature, Kelvin
    value = models.FloatField(
        "Температура поверхности океана, К", null=True)


# Chlorophyll model
class Chlorophyll(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Chlorophyll concentration, OCI algorithm, mg m^-3'
    value = models.FloatField("Концентрация хлорофила", null=True)


# Sea-ice cover model
class SeaIceCover(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Sea-ice cover, 0 - 1
    value = models.FloatField("Ледяное покрытие океана, да - нет", null=True)


# 2 metre temperature
class Temperature(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # 2 metre temperature, Kelvin
    value = models.FloatField("Температура у поверхности, К", null=True)


# Total cloud cover
class TotalCloudCover(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Total cloud cover
    value = models.FloatField("Облачность", null=True)


# Total cloud cover
class SeaWindSpeed(models.Model):
    # Longitude and latitude geo point
    point = models.PointField(default=Point(6, 70))
    # Datetime value
    datetime = models.DateTimeField(default=timezone.now, db_index=True)

    # Sea surface wind speed, meters
    value = models.FloatField(
        "Скорость ветра у поверхности океана, м", null=True)


#####################################################################

# Adding auth token on user save
@receiver(post_save, sender=settings.AUTH_USER_MODEL)
def create_auth_token(sender, instance=None, created=False, **kwargs):
    if created:
        Token.objects.create(user=instance)

# Define main characteristics and save then to data base
characteristics = [
    {
        "title": 'Sea surface temperature',
        "description": 'Sea surface temperature, Kelvin',
        "model": 'SeaSurfaceTemperature',
        "tag": 'sst',
        "storePrefix": 'modis-terra-l2',
        "fileMask": 'L2_LAC_SST',
    },
    {
        "title": 'Chlorophyll',
        "description": 'Chlorophyll concentration, OCI algorithm, mg m^-3',
        "model": 'Chlorophyll',
        "tag": 'chlor_a',
        "storePrefix": 'modis-terra-l2',
        "fileMask": 'L2_LAC_OC',
    },
    {
        "title": 'Bathymetric',
        "description": 'Bathymetric height - sea floor height, meters',
        "model": 'Bathymetric',
        "tag": 'elevation',
        "storePrefix": 'bathymetric',
        "fileMask": '',
    },
    {
        "title": 'Sea-ice cover',
        "description": 'Sea-ice cover, 0 - 1',
        "model": 'SeaIceCover',
        "tag": 'ci',
        "storePrefix": 'ci-t2m-tcc',
        "fileMask": '',
    },
    {
        "title": 'Temperature',
        "description": '2 metre temperature, Kelvin',
        "model": 'Temperature',
        "tag": 't2m',
        "storePrefix": 'ci-t2m-tcc',
        "fileMask": '',
    },
    {
        "title": 'Total cloud cover',
        "description": 'Total cloud cover',
        "model": 'TotalCloudCover',
        "tag": 'tcc',
        "storePrefix": 'ci-t2m-tcc',
        "fileMask": '',
    },
    {
        "title": 'Sea wind speed',
        "description": 'Sea surface wind speed, meters',
        "model": 'SeaWindSpeed',
        "tag": 'wind',
        "storePrefix": 'wind',
        "fileMask": '',
    },
]
for c in characteristics:
    try:
        Characteristic._meta.get_field("model")
        try:
            qs = Characteristic.objects.get(
                model = c["model"],
            )
        except:
            m = Characteristic(
                title = c["title"],
                description = c["description"],
                model = c["model"],
                tag = c["tag"],
                store_prefix = c["storePrefix"],
                file_mask = c["fileMask"],
            )
            m.save()
            print("Main characteristic '%s' created" % (c["title"]))
    except:
        print("No characteristic model found. Try to make DB migrations.")
