from django.contrib import admin
from .models import PowerSupply

class PowerSupplyAdmin(admin.ModelAdmin):
    model = PowerSupply
    list_display = ['address', 'time_unit', 'initial_time', 'start_time', 'minute_price']
