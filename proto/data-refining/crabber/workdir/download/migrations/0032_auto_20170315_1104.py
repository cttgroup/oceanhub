# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-15 11:04
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0031_auto_20170314_1605'),
    ]

    operations = [
        migrations.AlterField(
            model_name='task',
            name='files_names_generator',
            field=models.TextField(default='', verbose_name='Список имён файлов или генератор имен файлов для загрузки         (предполагает использование атрибута date)'),
        ),
    ]
