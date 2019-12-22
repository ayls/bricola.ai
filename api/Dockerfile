FROM python:3.6

WORKDIR /usr/app/
COPY . /usr/app/
EXPOSE 5000

RUN pip install -r api-requirements.txt
CMD python app.py