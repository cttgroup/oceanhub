import datetime

from django.db import models
from django.utils import timezone

import download.download as download
from app.settings import zredis


class Server(models.Model):
    slug = models.SlugField(
        "Уникальное обозначение сервера", default='', unique=True)
    url = models.CharField(
        "Адрес сервера", max_length=500, default='')
    max_connections = models.PositiveSmallIntegerField(
        "Максимальное кол-во соединений", default=1)
    passes = models.PositiveSmallIntegerField(
        "Кол-во проходов", default=3)

    def __str__(self):
        return self.slug

    """
    def save(self):
        super(Server, self).save()
        zredis.set(self.slug, 0)
    """


class StorePrefix(models.Model):
    title = models.CharField(
        "Описание", max_length=200, default='')
    slug = models.SlugField(
        "Уникальное обозначение", default='', unique=True, blank=True)

    def __str__(self):
        out = self.title if self.slug == '' else self.slug
        return out


class Task(models.Model):
    timestamp = models.DateTimeField(
        auto_now_add=True)
    date_start = models.DateField(
        "Период, с", default=None)
    date_end = models.DateField(
        "Период, по", default=None, blank=True, null=True)
    server = models.ForeignKey(
        'Server')
    store_prefix = models.ForeignKey(
        'StorePrefix')
    files_names_generator = models.TextField(
        "В формате Python: список имён файлов для загрузки; \
        или генератор имен файлов \
        (предполагает использование атрибута date)", default='')
    status = models.CharField(
        "Статут выполнения", max_length=50, default='new',
        editable=False)
    note = models.CharField(
        "Примечание", max_length=500, default='', blank=True)
    result = models.TextField(
        "Результат", default='', blank=True)
    run = models.BooleanField(
        "Выполнить", default=False)

    def __str__(self):
        return str(self.timestamp.strftime('%H:%M:%S-%Y%m%d'))

    def save(self):
        result = ''

        if self.run:
            result = download.download(
                self.server.url,
                self.server.max_connections,
                self.server.passes,
                self.date_start,
                self.date_end,
                self.store_prefix.slug,
                self.files_names_generator,
                self.status,
            )
            self.run = False
            self.result = result
        super(Task, self).save()
