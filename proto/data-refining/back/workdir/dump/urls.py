# dump
from django.conf.urls import url

from . import views


urlpatterns = [
    url(r'^test/(.+)', views.test),

    url(r'^create/', views.create),
    url(r'^update/', views.update),
    url(r'^get/', views.get),

    url(r'^api/tasks/$', views.DumpTaskList.as_view()), url(r'^api/tasks/(?P<pk>[0-9]+)/$', views.DumpTaskDetail.as_view()),
]
