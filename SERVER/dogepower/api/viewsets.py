from rest_framework.permissions import IsAuthenticated
from block_io import BlockIo
from datetime import timedelta
from rest_framework import viewsets
from rest_framework import mixins
from .serializers import PowerSupplySerializer
from rest_framework.response import Response
from django.utils import timezone
from .models import PowerSupply

import decimal

class PowerSupplyViewSet(viewsets.GenericViewSet, mixins.CreateModelMixin, mixins.UpdateModelMixin,):
    serializer_class = PowerSupplySerializer
    http_method_names = ['get', 'post']
    queryset = PowerSupply.objects.all()
    
    
    def get(self, request, pk=None, address=None):
        bio = BlockIo('1917-5233-735e-5d5c', 'tarticulatio951bretagne', 2)
        if pk:
            try:
                power_supplies_queryset = PowerSupply.objects.filter(address=pk)
                serializer = PowerSupplySerializer(
                    power_supplies_queryset, many=True)

                last_power_supply = power_supplies_queryset.filter(valid=True).last()
                if last_power_supply:
                    wallet_amount = decimal.Decimal(bio.get_address_balance(addresses='%s' % last_power_supply.address)['data']['available_balance'])
                    price = last_power_supply.minute_price * 60 * last_power_supply.initial_time if last_power_supply.time_unit == 'H' \
                        else last_power_supply.minute_price * 60 * 12 * last_power_supply.initial_time
                    if wallet_amount > last_power_supply.last_device_amount and wallet_amount > last_power_supply.last_device_amount / last_power_supply.minute_price:
                        time_paid_sec = int(wallet_amount - last_power_supply.last_device_amount / last_power_supply.minute_price) * 60
                        if last_power_supply.payment_expires <= timezone.now():
                            last_power_supply.payment_expires = timezone.now() + timedelta(seconds=time_paid_sec)
                        else:
                            last_power_supply.payment_expires = last_power_supply.payment_expires + timedelta(seconds=time_paid_sec)
                        last_power_supply.last_device_amount += wallet_amount
                        last_power_supply.paid = True
                        last_power_supply.save()
                sec_left = last_power_supply.payment_expires - timezone.now() if last_power_supply and last_power_supply.payment_expires > timezone.now() else None
                return Response({
                    'data': serializer.data,
                    'payment_expires': last_power_supply.payment_expires.timestamp() if last_power_supply and last_power_supply.payment_expires else timezone.now().timestamp(),
                    'seconds_left': sec_left.seconds if last_power_supply and last_power_supply.payment_expires > timezone.now() else 0
                })
            except PowerSupply.DoesNotExist:
                Response({'error': 'address not found'})
        else:
            new_address = bio.get_new_address()
            if new_address and new_address['status'] == "success":
                new_address_obj = PowerSupply.objects.create(
                    address=new_address['data']['address'],
                )
                return Response({'new_address': new_address_obj.address})
            else:
                return Response({'error': 'error getting an address'})

    def post(self, request, pk=None, address=None, test=None):
        new_power_supply = PowerSupply.objects.create(address=pk)
        power_supplies_queryset = PowerSupply.objects.filter(address=pk)
        last_power_supply = power_supplies_queryset.filter(valid=True).last()
        if last_power_supply:
            new_power_supply.last_device_amount = last_power_supply.last_device_amount
            new_power_supply.save()
        serializer = PowerSupplySerializer(
            power_supplies_queryset, many=True)
        return Response(serializer.data)
        
        

