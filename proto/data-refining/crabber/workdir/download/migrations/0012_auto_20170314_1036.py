# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-14 10:36
from __future__ import unicode_literals

from django.db import migrations, models
import django.utils.timezone


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0011_task_timestamp'),
    ]

    operations = [
        migrations.AlterField(
            model_name='task',
            name='date_end',
            field=models.DateField(default=django.utils.timezone.now, verbose_name='Период, по'),
        ),
    ]
