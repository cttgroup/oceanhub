# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-14 12:18
from __future__ import unicode_literals

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0015_server_passes'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='server',
            name='link',
        ),
        migrations.AddField(
            model_name='server',
            name='url',
            field=models.CharField(default='', max_length=500, verbose_name='Адрес'),
        ),
    ]
