# -*- coding: utf-8 -*-
# Generated by Django 1.11 on 2017-06-01 11:27
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('dump', '0004_auto_20170529_1536'),
    ]

    operations = [
        migrations.AddField(
            model_name='dumptask',
            name='elapsed_time',
            field=models.CharField(blank=True, default='', max_length=10, verbose_name='Время выполнения'),
        ),
    ]
