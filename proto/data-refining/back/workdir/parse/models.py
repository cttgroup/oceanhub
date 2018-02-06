# parse
import datetime
import os

from django.db import models
from django.utils import timezone

from ocean.models import Region
from ocean.models import Characteristic
from .parser import parse_byday, parse_set, parse_test


class Task(models.Model):
    timestamp = models.DateTimeField(
        auto_now_add=True)
    PROCESS_TYPES = (
        ('byday', 'Файлы разложены по дням'),
        ('set', 'Один файл'),
    )
    process_type = models.CharField(
        'Тип обработки данных',
        max_length=50,
        choices=PROCESS_TYPES,
        default='byday',
        )
    path = models.CharField('Путь к файлу для обработки',
        max_length=200, blank=True, default='',)
    date_start = models.DateField(
        'Период, с', default=None, blank=True, null=True)
    date_end = models.DateField(
        'Период, по', default=None, blank=True, null=True)
    characteristic = models.ForeignKey(
        Characteristic)
    region = models.ForeignKey(
        Region)
    status = models.CharField(
        'Статут выполнения', max_length=50, default='new',
        editable=False)
    result = models.TextField(
        "Результат", default='', blank=True)
    run = models.BooleanField(
        "Выполнить", default=False)

    def __str__(self):
        return str(self.timestamp.strftime('%H:%M:%S-%Y%m%d'))

    def save(self):
        result = ''

        if self.run:
            if self.process_type == 'set':
                result = parse_set(
                # result = parse_test(
                    self.path,
                    self.date_start,
                    self.date_end,
                    self.characteristic.model,
                    self.characteristic.tag,
                    self.characteristic.store_prefix,
                    self.characteristic.file_mask,
                    self.region.slug,
                )
            elif self.process_type == 'byday':
                result = parse_byday(
                    self.date_start,
                    self.date_end,
                    self.characteristic.model,
                    self.characteristic.tag,
                    self.characteristic.store_prefix,
                    self.characteristic.file_mask,
                    self.region.slug,
                )
            self.run = False
            self.result = result
        super(Task, self).save()
