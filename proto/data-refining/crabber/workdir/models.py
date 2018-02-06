# -*- coding: utf-8 -*- 
import datetime
import time
import random

from django.utils import timezone
from django.core.files.base import ContentFile
from django.db import models
from django.db.models.signals import pre_save
from django.db.models.signals import post_save
from django.db.models.signals import post_init
from django.db.models.signals import pre_init
from django.dispatch import receiver
from django.http import HttpResponseRedirect
from django.http import QueryDict
from django.forms import ModelForm
from django.contrib.auth.models import User

import library

from irepo.settings import BASE_DIR, MEDIA_ROOT


# модель данных: Установка для экспериментов
class Ustanovka(models.Model):
    def __unicode__(self):  # Python 3: def __str__(self):
        return self.title
    
    class Meta:
        verbose_name = "Установка"
        verbose_name_plural = "Установки"

    title = models.CharField(u"Обозначение", blank=True, max_length=200)


# модель данных: Материал иллюминатора
class MaterialIlluminatora(models.Model):
    def __unicode__(self):  # Python 3: def __str__(self):
        return self.title

    class Meta:
        verbose_name = "Материал иллюминатора"
        verbose_name_plural = "Материалы иллюминаторов"

    title = models.CharField(u"Обозначение", blank=True, max_length=200)


# модель данных: Рабочий газ
class RabochiyGaz(models.Model):
    def __unicode__(self):  # Python 3: def __str__(self):
        return self.title

    class Meta:
        verbose_name = "Рабочий газ"
        verbose_name_plural = "Рабочие газы"

    title = models.CharField(u"Обозначение", blank=True, max_length=200)


class Sotrudnik(User):
    class Meta:
        proxy = True
        ordering = ['last_name']
        verbose_name = u"Сотрудник"
        verbose_name_plural = u"Сотрудники"

    def __unicode__(self):  # Python 3: def __str__(self):
        name = ''
        if self.last_name:
            name = self.last_name + ' ' + self.first_name
        else:
            name = self.username
        return name


# модель данных: Эксперимент
class Exp(models.Model):
    def __unicode__(self):  # Python 3: def __str__(self):
        return self.title

    def was_published_recently(self):
        return self.date >= timezone.now() #- datetime.timedelta(days=1)
    was_published_recently.admin_order_field = u"Дата проведения эксперимента"
    was_published_recently.boolean = True
    was_published_recently.short_description = u'Недавние?'

    class Meta:
        verbose_name = "Эксперимент"
        verbose_name_plural = "Эксперименты"

    POYASNITELNAYAZAPISKA = ( (u'да', u'да'), (u'нет', u'нет'),)
    POLOZHENIEMODELI = ( (u'слева', u'слева'), (u'посередине', u'посередине'), (u'справа', u'справа'),)

    title = models.CharField(u"Идентифицирующее название эксперимента", max_length=200)
    date = models.DateTimeField(u"Дата проведения эксперимента", blank=True, null=True, auto_now_add=True)
    fileSTL = models.FileField(u"Файл stl", upload_to="exp", null=True, blank=True)
    fileXYZ = models.FileField(u"Файл xyz", upload_to="exp", null=True, blank=True)
    kolichestvoKamer = models.CharField(u"Количество камер", blank=True, max_length=200)
    otvetstvenniyZaExperiment = models.ForeignKey(Sotrudnik,related_name='sotrudnikOtvetstvenniyZaExperiment', verbose_name=u"Ответственный за эксперимент", blank=True, null=True)
    otvetstvenniyZaDannie = models.ForeignKey(Sotrudnik, related_name='sotrudnikOtvetstvenniyZaDannie', verbose_name=u"Ответственный за данные", blank=True, null=True)
    ustanovka = models.ForeignKey(Ustanovka, null=True, blank=True)
    poyasnitelnayaZapiska = models.CharField(u"Пояснительная записка", blank=True, max_length=200, choices=POYASNITELNAYAZAPISKA)
    materialIlluminatora = models.ForeignKey(MaterialIlluminatora, null=True, blank=True)
    rabochiygaz = models.ForeignKey(RabochiyGaz, null=True, blank=True)
    uslovnoeNaimenovanieIzdeliya = models.CharField(u"Условное наименование изделия", blank=True, max_length=200)
    tip3dModeli = models.CharField(u"Тип 3D модели", blank=True, max_length=200)
    rolikiTermosiomki = models.CharField(u"Ролики термосъемки", blank=True, max_length=200)
    sechenieIlluminatora = models.CharField(u"Сечение иллюминатора", blank=True, max_length=200)
    tolshinaIlluminatora = models.CharField(u"Толщина иллюминатора", blank=True, max_length=200)
    temperaturaTormozheniya = models.CharField(u"Температура торможения", blank=True, max_length=200)
    harakteristikiSopla = models.CharField(u"Характеристики сопла", blank=True, max_length=200)
    dlitelnost = models.CharField(u"Длительность", blank=True, max_length=200)
    davlenie = models.CharField(u"Давление", blank=True, max_length=200)
    teplovoiPotok = models.CharField(u"Тепловой поток", blank=True, max_length=200)
    polozhenieModeli = models.CharField(u"Положение модели", blank=True, max_length=200, choices=POLOZHENIEMODELI)
    plotnostMateriala = models.CharField(u"Плотность материала, кг/м3", blank=True, max_length=200)
    udelnajaTeploemkostMateriala = models.CharField(u"Удельная теплоемкость материала, Дж/(кг·К)", blank=True, max_length=200)
    teploprovodnostMateriala = models.CharField(u"Tеплопроводность материала, Вт/(м·К)", blank=True, max_length=200)


# модель данных: Тепловая камера
class Cam(models.Model):
    counter = 0

    def image_img(self):
        return '<img src="" />'

    image_img.short_description = 'Thumb'
    image_img.allow_tags = True

    def __unicode__(self):  # Python 3: def __str__(self):
        return self.modelKamery

    def save(self):
        super(Cam, self).save()
        data = library.parse_cam(self)
        root = data['root']
        self.modelKamery = data['modelKamery']
        self.razmerPikselja = root.attrib['pixelSize']
        self.distancijaSjomki = root.attrib['shootingDistance']
        self.fokusnoeRasstojanie = root.attrib['focalLength']
        self.apertura = root.attrib['detectorApperture']
        self.temperaturaSredy = root.attrib['atmosphericTemperature']
        self.prozrachnostSredy = root.attrib['atmosphericTransmission']
        self.temperaturaFona = root.attrib['backgroundTemperature']
        self.stepenChernoty = root.attrib['physicalEmissivity']
        self.temperaturnajaShkala = root.attrib['temperatureScale']
        self.kolvoTochekvStroke = root.attrib['frameDimensionColumns']
        self.kolvoStrok = root.attrib['frameDimensionRows']
        self.kolvoKadrovvRolike = root.attrib['framesTotally']
        self.dataiVremjaSjomki = root.attrib['dateTime']

        #print self.counter
        if self.counter == 0:
            self.counter += 1
            thumbPath = 'thumb_' + data['modelKamery'] + '_' + str(self.exp_id) + '.bmp'
            self.fileDataThumb.save(thumbPath, ContentFile(data['thumb']))
        #print self.counter

        #self.fileDataThumb.save('thumb_.bmp', ContentFile(data['thumb']))
        #self.fileDataThumb = data['thumb']
        super(Cam, self).save()

    POLOZHENIEKAMERY = ( (u'1', u'1'), (u'2', u'2'), (u'3', u'3'), (u'4', u'4'), (u'5', u'5'), (u'6', u'6'), (u'7', u'7'),)
    MODELKAMERY = ( (u'7700', u'7700'), (u'7750', u'7750'), (u'A40', u'A40'),)

    exp = models.ForeignKey(Exp)
    polozhenieKamery = models.CharField(u"Положение камеры", max_length=200, choices=POLOZHENIEKAMERY)
    fileData = models.FileField(u"Файл с данными тепловой камеры", upload_to="cam", null=True, blank=True)
    fileDataThumb = models.ImageField(u"Превью файла с данными", upload_to="cam", null=True, blank=True)
    modelKamery = models.CharField(u"Модель камеры", blank=True, max_length=200, choices=MODELKAMERY)
    dataiVremjaSjomki = models.CharField(u"Дата и время съёмки", blank=True, max_length=200)
    kolvoKadrovvRolike = models.CharField(u"Кол-во кадров в ролике", blank=True, max_length=200)
    kolvoStrok = models.CharField(u"Кол-во строк", blank=True, max_length=200)
    kolvoTochekvStroke = models.CharField(u"Кол-во точек в строке", blank=True, max_length=200)
    temperaturnajaShkala = models.CharField(u"Температурная шкала (K, C, F)", blank=True, max_length=200)
    stepenChernoty = models.CharField(u"Степень черноты", blank=True, max_length=200)
    temperaturaFona = models.CharField(u"Температура фона", blank=True, max_length=200)
    prozrachnostSredy = models.CharField(u"Прозрачность среды", blank=True, max_length=200)
    temperaturaSredy = models.CharField(u"Температура среды", blank=True, max_length=200)
    distancijaSjomki = models.CharField(u"Дистанция съёмки", blank=True, max_length=200)
    razmerPikselja = models.CharField(u"Размер пикселя", blank=True, max_length=200)
    apertura = models.CharField(u"Апертура", blank=True, max_length=200)
    fokusnoeRasstojanie = models.CharField(u"Фокусное расстояние", blank=True, max_length=200)



# модель данных: Обработка результатов эксперимента
class Obr(models.Model):
    exp = models.ForeignKey(Exp)
    date = models.DateTimeField(u"Дата проведения обработки данных", auto_now=True)
    fileData = models.FileField(u"Файл с результатами обработки данных эксперимента", upload_to="voltherm_results", null=True, blank=True)
    status = models.CharField(u"Статус обработки эксперимента", blank=True, max_length=10)
    statusText = models.CharField(u"Описание статуса", blank=True, max_length=200)
    class Meta:
        ordering = ['-date']

    def __unicode__(self):  # Python 3: def __str__(self):
        return u"Результат обработки от " + self.date.strftime("%Y-%m-%d")

# модель данных: Индикатор обработки эксперимента
class ObrIndicator(models.Model):

    INDICATORSTAGE=(
        (u'1', u'Стадия 1'),
        (u'2', u'Стадия 2'),
        (u'3', u'Стадия 3'),
    )

    obr = models.ForeignKey(Obr, related_name="indicator")
    stage = models.CharField(u"Этап обработки", blank=True, max_length=200, choices=INDICATORSTAGE)
    procent = models.CharField(u"Процент выполнения", blank=True, max_length=200)

    class Meta:
        ordering = ['pk']

    def __unicode__(self):  # Python 3: def __str__(self):
        return u"Индикатор обработки № " + str(self.obr_id)


# модель данных: Параметры камер для обработки результатов эксперимента
class ObrCam(models.Model):
    obr = models.ForeignKey(Obr)
    temperaturnajaShkala = models.CharField(u"Температурная шкала (K, C, F)", blank=True, max_length=200)
    stepenChernoty = models.CharField(u"Степень черноты", blank=True, max_length=200)
    temperaturaFona = models.CharField(u"Температура фона", blank=True, max_length=200)
    prozrachnostSredy = models.CharField(u"Прозрачность среды", blank=True, max_length=200)
    distancijaSjomki = models.CharField(u"Дистанция съёмки", blank=True, max_length=200)
    fokusnoeRasstojanie = models.CharField(u"Фокусное расстояние", blank=True, max_length=200)
    razmerPikselja = models.CharField(u"Размер пикселя", blank=True, max_length=200)
    kolvoStrok = models.CharField(u"Кол-во строк", blank=True, max_length=200)
    kolvoTochekvStroke = models.CharField(u"Кол-во точек в строке", blank=True, max_length=200)
    pozicijaKameryX = models.CharField(blank=True, max_length=200)
    pozicijaKameryY = models.CharField(blank=True, max_length=200)
    pozicijaKameryZ = models.CharField(blank=True, max_length=200)
    ugolPovorotaA = models.CharField(blank=True, max_length=200)
    ugolPovorotaB = models.CharField(blank=True, max_length=200)
    ugolPovorotaG = models.CharField(blank=True, max_length=200)


