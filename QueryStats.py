#!/usr/bin/env python
# -*- coding: utf-8 -*-
'''
Created on Jan 22, 2015

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


def main(argv):
    id_url = SummonerInfo.BASE_URL + '/lol/static-data/na/v1.2/champion' + '/?champData=info' + '&api_key='+ SummonerInfo.API_KEY
    id_resp = SummonerInfo.get_response(id_url)
    champ_ids = {}
    for i in id_resp['data']:
        champ_ids[str(id_resp['data'][i]['id'])] = id_resp['data'][i]['name']
    names = []
    names.append(argv[1])
    response = SummonerInfo.get_response(SummonerInfo.build_url(names))
    if(response == False):
        return "error"
    ids = SummonerInfo.get_id(response)
    parsed = open(os.getcwd() + '/data_parsed.txt', 'w')
    for id in ids:
        url = get_url(id, argv[2])
        stats = SummonerInfo.get_response(url)
        if(stats == False):
            return "error"
        for k in stats['champions']:
            if(k['id'] == 0):
                continue
            parsed.write(champ_ids[str(k['id'])] + ' : ')
            for i in k['stats']:
                if i == 'totalSessionsPlayed':
                    parsed.write('Total Games Played: ' + str(k['stats'][i]) + '    ')
                    total = k['stats'][i]
                if i == 'totalSessionsWon':
                    parsed.write('Total Games Won: ' + str(k['stats'][i]) + '    ')
                    won = k['stats'][i]
                if i == 'totalSessionsLost':
                    parsed.write('Total Games Lost: ' + str(k['stats'][i]) + '    ')
            winrate = (won / total) * 100
            parsed.write(("Win Percentage: {0:.2f}" + "%").format(winrate))



            parsed.write('\n')

    parsed.close()

if __name__ == '__main__':
    main(sys.argv)
