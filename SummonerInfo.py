#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on Jan 21, 2015

@author: Hanna
'''

import json
import urllib.parse
import urllib.request

API_KEY = '**** YOUR API KEY HERE ****'
BASE_URL = 'https://na.api.pvp.net/api'

def build_url(summonerNames : list) -> str:
    parameters = ''
    length = len(summonerNames)
    parameters += summonerNames[0]
    for i in range(1, length-1):
        parameters += ','+summonerNames[i]
    return BASE_URL +'/lol/na/v1.4/summoner/by-name/'+parameters+'?api_key='+API_KEY

def get_response(url : str) -> 'json':
    response = None
    try:
        response = urllib.request.urlopen(url)
        json_text = response.read().decode(encoding = 'utf-8')
        return json.loads(json_text)
        #return json.dumps(json_text)
    except urllib.error.HTTPError:
        return False
    finally:
        if response != None:
            response.close()

def get_id(response: 'json') -> [int]:
    ids = []
    for k in response.keys():
        ids.append(response[k]['id'])
    return ids


