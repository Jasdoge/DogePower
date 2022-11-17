from django.db import models
from django.utils import timezone
from django.core.validators import MinLengthValidator

DAY='D'
HOUR='H'

TIME_UNIT_CHOICES = (
    (DAY, "day(s)"),
    (HOUR, "hour(s)"),
)

class PowerSupply(models.Model):
    address = models.CharField(validators=[MinLengthValidator(34)], max_length=34, verbose_name='address')
    time_unit = models.CharField(max_length=1, choices=TIME_UNIT_CHOICES, default=HOUR, verbose_name='time unit')
    initial_time = models.DecimalField(max_digits=5, decimal_places=1, default=1, verbose_name='initial time')
    start_time = models.DateTimeField(default=timezone.now(), verbose_name='start time')
    payment_expires = models.DateTimeField(default=timezone.now(), verbose_name='start time')
    minute_price = models.IntegerField(default=1, verbose_name='price per minute')
    valid = models.BooleanField(default=True)
    paid = models.BooleanField(default=False)
    last_device_amount = models.DecimalField(max_digits=5, decimal_places=2, default=0, verbose_name='amount on the machine')
    
