"""dogepower URL Configuration
"""
from django.contrib import admin
from django.urls import path
from rest_framework import routers
from api import viewsets

router = routers.DefaultRouter()

router.register(r"device", viewsets.PowerSupplyViewSet, basename="power-supply")
router.register(r"device/(?P<address>\w+)", viewsets.PowerSupplyViewSet, basename="power-supply-address")

urlpatterns = [
    path('addresses', viewsets.AddressView.as_view(), name="addresses"),
    path('admin/', admin.site.urls),
    *router.urls,
]
