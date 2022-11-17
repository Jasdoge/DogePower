# Generated by Django 4.1.3 on 2022-11-17 13:54

import datetime
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0002_powersupply_minute_price_and_more'),
    ]

    operations = [
        migrations.AddField(
            model_name='powersupply',
            name='last_device_amount',
            field=models.DecimalField(decimal_places=2, default=0, max_digits=5, verbose_name='amount on the machine'),
        ),
        migrations.AddField(
            model_name='powersupply',
            name='valid',
            field=models.BooleanField(default=True),
        ),
        migrations.AlterField(
            model_name='powersupply',
            name='start_time',
            field=models.DateTimeField(default=datetime.datetime(2022, 11, 17, 13, 54, 5, 830245, tzinfo=datetime.timezone.utc), verbose_name='start time'),
        ),
    ]
