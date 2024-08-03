#!/usr/bin/bash

sed -i 's/>=/|/g' tests/**
sed -i 's/!/\//g' tests/**
sed -i 's/,/@/g' tests/**
sed -i 's/;/,/g' tests/**
sed -i 's/@/;/g' tests/**
