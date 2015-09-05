"Go plugins. 
" Plug-in manager: https://github.com/junegunn/vim-plug
" Download: 
"  curl -fLo ~/.vim/autoload/plug.vim --create-dirs \
"      https://raw.githubusercontent.com/junegunn/vim-plug/master/plug.vim
" Then do: 
"  :PlugInstall

call plug#begin('~/.vim/plugged')

Plug 'https://github.com/fatih/vim-go.git'

call plug#end()


syntax on
filetype plugin indent on

set hlsearch
"set expandtab
"set shiftwidth=4
"set softtabstop=4
set tabstop=4
set autoindent
set number
set nocompatible
set backupcopy=yes
set bs=2
set viminfo='20,\"50
set history=50
set ruler
