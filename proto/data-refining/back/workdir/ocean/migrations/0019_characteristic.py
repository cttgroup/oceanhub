# -*- coding: utf-8 -*-
# Generated by Django 1.11 on 2017-05-19 08:39
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('ocean', '0018_seawindspeed'),
    ]

    operations = [
        migrations.CreateModel(
            name='Characteristic',
            fields=[
                ('id', models.AutoField(auto_created=True, primary_key=True, serialize=False, verbose_name='ID')),
                ('title', models.CharField(default='', max_length=200, verbose_name='Название')),
                ('model', models.CharField(default='', max_length=200, verbose_name='Модель данных')),
                ('tag', models.SlugField(default='', verbose_name='Опция для парсера (например, тэг в netCDF/hdf файле)')),
                ('store_prefix', models.SlugField(default='', verbose_name='Префикс хранения файлов')),
                ('file_mask', models.CharField(blank=True, default='', max_length=50, verbose_name='Маска поиска файла')),
            ],
        ),
    ]
