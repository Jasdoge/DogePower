from rest_framework.permissions import IsAuthenticated
from block_io import BlockIo
from datetime import timedelta
from rest_framework import mixins, views, viewsets
from .serializers import PowerSupplySerializer
from rest_framework.response import Response
from django.utils import timezone
from .models import PowerSupply

import decimal

class PowerSupplyViewSet(viewsets.GenericViewSet, mixins.CreateModelMixin, mixins.UpdateModelMixin,):
    # Power supply is a singleton created if no address is given in URL
    serializer_class = PowerSupplySerializer
    http_method_names = ['get']
    queryset = PowerSupply.objects.all()
    
    
    def get(self, request, pk=None, address=None):
        bio = BlockIo('xxxx-xxxx-xxxx-xxxx', 'apipassword', 2)
        if pk:
            try:
                power_supplies_queryset = PowerSupply.objects.filter(address=pk)
                serializer = PowerSupplySerializer(
                    power_supplies_queryset, many=True)
                last_power_supply = power_supplies_queryset.filter(valid=True).last()
                # wallet_amount is the amount on the wallet, it's compared to the last amount the device had
                wallet_amount = decimal.Decimal(bio.get_address_balance(addresses='%s' % last_power_supply.address)['data']['available_balance'])
                # be careful Doge Power don't give change !
                if wallet_amount > last_power_supply.last_device_amount:
                    # the time we got from payment is calculated and added in the 'payment_expires' attribute
                    time_paid_sec = int(wallet_amount - last_power_supply.last_device_amount / last_power_supply.minute_price) * 60
                    if last_power_supply.payment_expires <= timezone.now():
                        last_power_supply.payment_expires = timezone.now() + timedelta(seconds=time_paid_sec)
                    else:
                        last_power_supply.payment_expires = last_power_supply.payment_expires + timedelta(seconds=time_paid_sec)
                    # we save the new amount on the device
                    last_power_supply.last_device_amount += wallet_amount
                    # 'paid' and 'valid' attributes are not used yet
                    last_power_supply.paid = True
                    last_power_supply.save()
                # remaining time is converted to seconds
                sec_left = last_power_supply.payment_expires - timezone.now() if last_power_supply and last_power_supply.payment_expires > timezone.now() else None
                return Response({
                    'data': serializer.data,
                    'payment_expires': last_power_supply.payment_expires.timestamp() if last_power_supply and last_power_supply.payment_expires else timezone.now().timestamp(),
                    'seconds_left': sec_left.seconds if last_power_supply and last_power_supply.payment_expires > timezone.now() else 0
                })
            except PowerSupply.DoesNotExist:
                Response({'error': 'address not found'})
        else:
            # generation of a new address and a PowerSupply object singleton
            new_address = bio.get_new_address()
            if new_address and new_address['status'] == "success":
                new_address_obj = PowerSupply.objects.create(
                    address=new_address['data']['address'],
                )
                return Response({'new_address': new_address_obj.address})
            else:
                return Response({'error': 'error getting an address'})

class AddressView(views.APIView):
    # thiis view generates a list of all addressesavailable on the account
    serializer_class = PowerSupplySerializer
    http_method_names = ['get']
    queryset = PowerSupply.objects.all()

    def get(self, request):
        addresses_list = [ps.address for ps in PowerSupply.objects.all()]
        return Response(addresses_list)
        
        

