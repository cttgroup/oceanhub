# -*- coding: utf-8 -*-
# Generated by Django 1.11 on 2017-05-19 08:48
from __future__ import unicode_literals

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('parse', '0009_auto_20170519_0829'),
    ]

    operations = [
        migrations.RemoveField(
            model_name='task',
            name='characteristic',
        ),
        migrations.DeleteModel(
            name='Characteristic',
        ),
    ]
