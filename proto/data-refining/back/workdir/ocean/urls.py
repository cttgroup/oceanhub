# ocean
from django.conf.urls import url
from rest_framework.authtoken import views as apiViews

from ocean import views


urlpatterns = [
    url(r'^test/(.+)', views.test),

    url(r'^getfilter/', views.get_filter),

    # REST API
    # Obtain a token given the username and password
    url(r'^api-token-auth/', apiViews.obtain_auth_token),

    url(r'^api/regions/$', views.RegionsList.as_view()),
    # url(r'^api/regions/(?P<pk>[0-9]+)/$', views.RegionsDetail.as_view()),
    url(r'^api/regions/(?P<argument>[\w-]+)/$', views.regions_detail),
    url(r'^api/characteristics/$', views.CharacteristicsList.as_view()),
    url(r'^api/characteristics/(?P<pk>[0-9]+)/$', \
        views.CharacteristicsDetail.as_view()),

    url(r'^api/chlor' +
        '/(?P<date>[\w-]+)' +
        '/(?P<region>[\w-]+)' +
        '/(?P<needed>[\w-]+)' +
        '/$', views.ApiChlor.as_view()),
    url(r'^api/sst' +
        '/(?P<date>[\w-]+)' +
        '/(?P<region>[\w-]+)' +
        '/(?P<needed>[\w-]+)' +
        '/$', views.ApiSST.as_view()),
    url(r'^api/bath' +
        '/(?P<date>[\w-]+)' +
        '/(?P<region>[\w-]+)' +
        '/(?P<needed>[\w-]+)' +
        '/$', views.ApiBath.as_view()),


]
