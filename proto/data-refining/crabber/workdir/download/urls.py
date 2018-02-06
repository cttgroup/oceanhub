from django.conf.urls import url

from download import views

urlpatterns = [
    url(r'^([\w-]+)/$', views.download),
    # url(r'^(\d+)/(\d+)/([\w-]+)/$', views.download),
    url(r'^(?P<year>(19|20)\d{2})/(?P<day>\d+)/(?P<suffix>[\w-]+)/$',
        views.download),

    # 4 testing
    url(r'^test$', views.download),
]
