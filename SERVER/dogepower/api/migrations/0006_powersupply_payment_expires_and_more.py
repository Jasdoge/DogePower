# Generated by Django 4.1.3 on 2022-11-17 17:26

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0005_powersupply_paid_alter_powersupply_start_time'),
    ]

    operations = [
        migrations.AddField(
            model_name='powersupply',
            name='payment_expires',
            field=models.DateTimeField(default=datetime.datetime(2022, 11, 17, 17, 26, 5, 971745, tzinfo=datetime.timezone.utc), verbose_name='payment expires'),
        ),
        migrations.AlterField(
            model_name='powersupply',
            name='start_time',
            field=models.DateTimeField(default=datetime.datetime(2022, 11, 17, 17, 26, 5, 971727, tzinfo=datetime.timezone.utc), verbose_name='start time'),
        ),
    ]
