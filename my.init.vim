" Install vim-plug: https://github.com/junegunn/vim-plug#neovim
" Install coc.nvim: https://github.com/neoclide/coc.nvim
" To enable rustfmt on save:
" :CocConfig
" 
" {
"	"coc.preferences.formatOnSaveFiletypes": [
"		"rust"
"	}
" }

call plug#begin()
Plug 'neoclide/coc.nvim', {'branch': 'release'}
Plug 'petRUShka/vim-sage'
Plug 'jacoborus/tender.vim' "colorscheme tender
Plug 'savq/melange'         "colorscheme melange
call plug#end()

" Make <CR> to accept selected completion item or notify coc.nvim to format
" <C-g>u breaks current undo, please make your own choice
inoremap <silent><expr> <CR> coc#pum#visible() ? coc#pum#confirm()
                              \: "\<C-g>u\<CR>\<c-r>=coc#on_enter()\<CR>"

syntax enable
set termguicolors
colorscheme melange
set cursorline
set cursorcolumn
set number
set relativenumber
"set background=light
