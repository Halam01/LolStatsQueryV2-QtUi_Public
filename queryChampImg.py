#!/usr/bin/env python
# -*- coding: utf-8 -*-
import json
import urllib.parse
import urllib.request
import SummonerInfo
import sys
from PyQt4 import QtGui

'''
Created on Mar 6, 2017

@author: Hanna
'''
import json
import urllib.parse
import urllib.request
import SummonerInfo
import sys
import os
from PyQt4 import QtGui

def get_url(summonerID: int, season : str) -> str:
    IDstr = str(summonerID)
    return SummonerInfo.BASE_URL + '/lol/na/v1.3/stats/by-summoner/' + IDstr + '/ranked?season='+season+'&api_key='+ SummonerInfo.API_KEY

def main():
    v_url = SummonerInfo.BASE_URL + '/lol/static-data/na/v1.2/versions' + '?api_key='+ SummonerInfo.API_KEY
    v_resp = SummonerInfo.get_response(v_url)
    version = v_resp[0]
    print("Version: " + str(version))
    id_url = SummonerInfo.BASE_URL + '/lol/static-data/na/v1.2/champion' + '/?champData=image' + '&api_key='+ SummonerInfo.API_KEY
    id_resp = SummonerInfo.get_response(id_url)
    champ_ids = {}

    imgURL = 'http://ddragon.leagueoflegends.com/cdn/' + str(version) +'/img/champion/'
    for i in id_resp['data']:
        champ_ids[str(id_resp['data'][i]['name'])] = str(id_resp['data'][i]['image']['full'])
        urllib.request.urlretrieve(imgURL + str(id_resp['data'][i]['image']['full']) ,  os.getcwd() + '/champImgsFull/' + str(id_resp['data'][i]['image']['full']))


    splashURL = "http://ddragon.leagueoflegends.com/cdn/img/champion/splash/"     #Aatrox_0.jpg"
    for i in id_resp['data']:
        urllib.request.urlretrieve(splashURL + str(i).rstrip() + "_0.jpg", os.getcwd() + '/splashImgs/' + str(i).rstrip() + '.jpg')
if __name__ == '__main__':
    main()
