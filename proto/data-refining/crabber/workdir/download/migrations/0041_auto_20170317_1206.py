# -*- coding: utf-8 -*-
# Generated by Django 1.10.4 on 2017-03-17 12:06
from __future__ import unicode_literals

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('download', '0040_auto_20170317_1205'),
    ]

    operations = [
        migrations.AlterField(
            model_name='task',
            name='store_prefix',
            field=models.ForeignKey(blank=True, default='', on_delete=django.db.models.deletion.CASCADE, to='download.StorePrefix'),
        ),
    ]
