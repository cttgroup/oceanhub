# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-14 15:59
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0027_auto_20170314_1355'),
    ]

    operations = [
        migrations.AddField(
            model_name='task',
            name='file_names_generator',
            field=models.TextField(default='', verbose_name='Генератор имен файлов для загрузки'),
        ),
    ]
