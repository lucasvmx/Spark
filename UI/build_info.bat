:: Script para gerar informações sobre a data e a hora de compilação
:: By: Lucas Vieira de Jesus

@echo off
echo generating build info ...
ver > ..\..\info.bld
date /t >> ..\..\info.bld
time /t >> ..\..\info.bld