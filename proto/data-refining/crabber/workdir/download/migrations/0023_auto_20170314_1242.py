# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-14 12:42
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0022_auto_20170314_1241'),
    ]

    operations = [
        migrations.AlterField(
            model_name='server',
            name='site_url',
            field=models.CharField(default='', max_length=500, verbose_name='Адрес сервера'),
        ),
    ]
