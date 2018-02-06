## ocean_crabberdjango
import os
import redis


# Quick-start development settings - unsuitable for production
# See https://docs.djangoproject.com/en/1.9/howto/deployment/checklist/

# Build paths inside the project like this: os.path.join(BASE_DIR, ...)
BASE_DIR = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
# SECURITY WARNING: keep the secret key used in production secret!
SECRET_KEY = '(se*&del$j3f(E30=g&(8-_vq3u3mw*0w4o3w_(3az7l$p)7u#'
# SECURITY WARNING: don't run with debug turned on in production!
# DEBUG = False
DEBUG = True
ALLOWED_HOSTS = ['10.17.1.25', '127.0.0.1', ]


# Application definition
INSTALLED_APPS = [
    'django.contrib.admin',
    'django.contrib.auth',
    'django.contrib.contenttypes',
    'django.contrib.sessions',
    'django.contrib.messages',
    'django.contrib.staticfiles',
    'django.contrib.gis',

    # Plugging aditional applications
    'rest_framework',
    'corsheaders',

    # Plugging own applications
    'download',
]


MIDDLEWARE_CLASSES = [
    'django.middleware.security.SecurityMiddleware',
    'django.contrib.sessions.middleware.SessionMiddleware',
    'django.middleware.common.CommonMiddleware',
    'django.middleware.csrf.CsrfViewMiddleware',
    'django.contrib.auth.middleware.AuthenticationMiddleware',
    'django.contrib.auth.middleware.SessionAuthenticationMiddleware',
    'django.contrib.messages.middleware.MessageMiddleware',
    'django.middleware.clickjacking.XFrameOptionsMiddleware',

    # Django CORS
    'corsheaders.middleware.CorsMiddleware',
]


ROOT_URLCONF = 'app.urls'


TEMPLATES = [
    {
        'BACKEND': 'django.template.backends.django.DjangoTemplates',
        'DIRS': [],
        'APP_DIRS': True,
        'OPTIONS': {
            'context_processors': [
                'django.template.context_processors.debug',
                'django.template.context_processors.request',
                'django.contrib.auth.context_processors.auth',
                'django.contrib.messages.context_processors.messages',
            ],
        },
    },
]


WSGI_APPLICATION = 'app.wsgi.application'


# Database
# https://docs.djangoproject.com/en/1.9/ref/settings/#databases
DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.sqlite3',
        'NAME': os.path.join(BASE_DIR, 'dev.sqlite3'),
    }
}

# Password validation
# https://docs.djangoproject.com/en/1.9/ref/settings/#auth-password-validators
AUTH_PASSWORD_VALIDATORS = [
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.UserAttributeSimilarityValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.MinimumLengthValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.CommonPasswordValidator',
    },
    {
        'NAME': 'django.contrib.auth.password_validation'
        '.NumericPasswordValidator',
    },
]


# Internationalization
# https://docs.djangoproject.com/en/1.9/topics/i18n/
LANGUAGE_CODE = 'ru-ru'
TIME_ZONE = 'UTC'
USE_I18N = True
USE_L10N = True
USE_TZ = True


# Static files (CSS, JavaScript, Images)
# https://docs.djangoproject.com/en/1.9/howto/static-files/
STATIC_URL = '/static/'
# URL_STATIC_ROOT = os.path.join(BASE_DIR, 'static')
STATICFILES_DIRS = (
    # Using with build-in web-server
    # os.path.join(BASE_DIR, 'static'),
)
# Using with web-server
STATIC_ROOT = os.path.join(BASE_DIR, "static/")


# MEDIA_URL = '/media/'


# Turning off cache on development
CACHES = {
    'default': {
        'BACKEND': 'django.core.cache.backends.dummy.DummyCache',
    }
}


# CORS stuff
CORS_ORIGIN_ALLOW_ALL = True
"""
CORS_ORIGIN_WHITELIST = (
    '127.0.0.1:3000',
    '127.0.0.1:5000',
)
"""


# Directory for storing data corresponding to docker-compose.yml
DATA_DIR = '/data/'


# Admin localization settings
ADMIN_SITE_HEADER = "Система автоматизированной загрузки данных"
ADMIN_SITE_TITLE = "Система автоматизированной загрузки данных"
ADMIN_INDEX_TITLE = "Загрузка данных"


# Celery stuff
zredis = redis.StrictRedis(host='crabberredis', port=6379, db=0)
# MAX_WGET_WAIT = 600
BROKER_URL = 'redis://crabberredis:6379'
CELERY_RESULT_BACKEND = 'redis://crabberredis:6379'
CELERY_ACCEPT_CONTENT = ['application/json']
CELERY_TASK_SERIALIZER = 'json'
CELERY_RESULT_SERIALIZER = 'json'
CELERY_TIMEZONE = 'Europe/Moscow'


# Try to import local settings from local_settings.py
try:
    from app.local_settings import *
except ImportError:
    pass
