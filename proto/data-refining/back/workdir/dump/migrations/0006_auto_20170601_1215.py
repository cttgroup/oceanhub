# -*- coding: utf-8 -*-
# Generated by Django 1.11 on 2017-06-01 12:15
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('dump', '0005_dumptask_elapsed_time'),
    ]

    operations = [
        migrations.AlterField(
            model_name='dumptask',
            name='elapsed_time',
            field=models.CharField(blank=True, default='', max_length=10, verbose_name='Время выполнения, секунды'),
        ),
    ]
