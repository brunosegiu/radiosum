%%
%% This is file `udelar.cls',
%% generated with the docstrip utility.
%%
%% The original source files were:
%%
%% coppe.dtx  (with options: `class')
%% 
%% This is the `udelar' document class used for composition of LaTeX
%% monographs (master and PhD thesis, and PhD qualifying exams)
%% satisfying the requirements of the Comisión Académica de Posgrado (CAP)
%% at the Universidad de la República (UdelaR).
%% 
%% Copyright (C) 2015 Pablo Castrillo Green   URL http://www.fing.edu.uy/~pabloc/
%% Copyright (C) 2015 Mihdí Anthony Caballero Baher   URL http://www.fing.edu.uy/~mcaballero/
%% 
%% This program is free software; you can redistribute it and/or modify
%% it under the terms of the GNU General Public License version 3 as
%% published by the Free Software Foundation.
%% 
%% This program is distributed in the hope that it will be useful,
%% but WITHOUT ANY WARRANTY; without even the implied warranty of
%% MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
%% GNU General Public License version 3 for more details.
%% 
%% You should have received a copy of the GNU General Public License
%% version 3 along with this package (see COPYING file).
%% If not, see <http://www.gnu.org/licenses/>.
%% 
%% Author(s): Pablo Castrillo Green  / Mihdí Caballero
%%
%% Current developer: Mihdí Caballero
%% 
%% {1417}
%% \CharacterTable
%%  {Upper-case    \A\B\C\D\E\F\G\H\I\J\K\L\M\N\O\P\Q\R\S\T\U\V\W\X\Y\Z
%%   Lower-case    \a\b\c\d\e\f\g\h\i\j\k\l\m\n\o\p\q\r\s\t\u\v\w\x\y\z
%%   Digits        \0\1\2\3\4\5\6\7\8\9
%%   Exclamation   \!     Double quote  \"     Hash (number) \#
%%   Dollar        \$     Percent       \%     Ampersand     \&
%%   Acute accent  \'     Left paren    \(     Right paren   \)
%%   Asterisk      \*     Plus          \+     Comma         \,
%%   Minus         \-     Point         \.     Solidus       \/
%%   Colon         \:     Semicolon     \;     Less than     \<
%%   Equals        \=     Greater than  \>     Question mark \?
%%   Commercial at \@     Left bracket  \[     Backslash     \\
%%   Right bracket \]     Circumflex    \^     Underscore    \_
%%   Grave accent  \`     Left brace    \{     Vertical bar  \|
%%   Right brace   \}     Tilde         \~}
%%

\def\filename{udelar.dtx}
\def\fileversion{v2.2.3}
\def\filedate{2015/10/10}
\NeedsTeXFormat{LaTeX2e}[1995/12/01]
\ProvidesClass{udelar}[\filedate\ \fileversion\ Fing Dissertations and Thesis]
\LoadClass[12pt,a4paper]{book}

%=========================
%==== INICIO PAQUETES ====
%=========================

\RequirePackage{ulem} % Para configurar simple control de cambios
\RequirePackage{color}
\RequirePackage{hyphenat}
\RequirePackage[toc]{appendix}
\RequirePackage{lastpage}
\RequirePackage[spanish,es-nodecimaldot]{babel}
\RequirePackage[utf8x]{inputenc}
\RequirePackage{amsmath,amssymb}
\RequirePackage{ifthen}
\RequirePackage{graphicx}
\RequirePackage{float}
\RequirePackage{caption}
\RequirePackage[font=small,labelfont=bf]{caption}
\RequirePackage{setspace}
\RequirePackage{tabularx}
\RequirePackage{eqparbox}
\RequirePackage{ltxcmds}
\RequirePackage{titletoc}
\RequirePackage[colorlinks,
            citecolor=blue,
            urlcolor=magenta,
            linkcolor=red,
            citebordercolor={0 0 1},
            urlbordercolor={0 1 1},
            linktocpage,
            hyperfootnotes=true
            ]{hyperref} %[colorlinks]
\RequirePackage[a4paper,bindingoffset=0.0cm,vcentering=true,%
top=4cm,bottom=3cm,left=4.0cm,right=3.0cm]{geometry} %showframe para ver los margins
\RequirePackage[pagewise]{lineno}
\RequirePackage[printwatermark]{xwatermark}
\RequirePackage{xcolor}
\RequirePackage[nottoc]{tocbibind}

\DeclareFontShape{OT1}{cmr}{bx}{sc}{<-> cmbcsc10}{} % Magia por error con 0T1 --> http://tex.stackexchange.com/questions/37618/a-problem-with-textsc

\RequirePackage{listings}
\RequirePackage{subcaption}
\RequirePackage{pgfplots}
\RequirePackage{multirow}
\RequirePackage{longtable}
\RequirePackage{pbox}
\RequirePackage{smartdiagram} 
\RequirePackage[colorlinks]{hyperref}
\RequirePackage{csvsimple}
\RequirePackage{filecontents}
\RequirePackage{booktabs}
\RequirePackage{tablefootnote}
\RequirePackage{tabularx}
\RequirePackage{ltablex} 
\RequirePackage{longtable}
\RequirePackage{pdflscape}
\RequirePackage{changepage}
\RequirePackage[a4paper]{geometry}
\RequirePackage[mode=buildnew]{standalone}% requires -shell-escape
\RequirePackage{tikz}
\RequirePackage{algorithm} 
\RequirePackage{algpseudocode} 
\RequirePackage{array}
\RequirePackage{afterpage}
\RequirePackage{smartdiagram} % Diagramas http://www.texample.net
\RequirePackage[square,numbers]{natbib}
\RequirePackage[
acronyms, 											 %utiliza el º de acronimos.
nohypertypes={acronym,notacion,simbolos,glosario},   %quita los links en el texto al glosario.
%nonumberlist,                                       %quita los links en los glosarios al texto.
nogroupskip,                                         %quita los espacios entre diferentes grupos dentro de un glosario.
nopostdot, 											 %quita el punto final en los acrónimos         .                           
]{glossaries}


%======================
%==== FIN PAQUETES ====
%======================

\DeclareOption{oneside}{
\setboolean{@twoside}{false}}

\DeclareOption{twoside}{
\setboolean{@twoside}{true}}

\newboolean{linenumbers}
\newboolean{watermark}

\DeclareOption{linenumbers}{
\setboolean{linenumbers}{true}}

\DeclareOption{watermark}{
\setboolean{watermark}{true}}

%================================
%==== INICIO NUEVOS COMANDOS ====
%================================

\addto\captionsspanish{\renewcommand{\tablename}{Tabla}}
\addto\captionsspanish{\renewcommand{\listtablename}{Lista de tablas}}
\addto\captionsspanish{\renewcommand{\listfigurename}{Lista de figuras}}
\addto\captionsspanish{\renewcommand{\contentsname}{Tabla de contenidos}}
\addto\captionsspanish{\renewcommand{\bibname}{Referencias bibliográficas}}

% Comandos para control de cambios
\newcommand{\revision}[1]{{\color{blue}{}#1}} % Para escribir texto nuevo de la revisión.
\newcommand{\eliminar}[1]{{\color{red}\sout{#1}}} % Para marcar como eliminado el texto actual.

%%==== APACITE ====
%\renewcommand{\BOthers}[1]{et al.\hbox{}}
%\renewcommand{\BBAA}{\&}% Letra que va entre los autores en las referencias
%\renewcommand{\BBAB}{and}% Entre autores en el texto
%%=================

%==== Comando UdelaRTeX ====
\def \UdelaRTeX {{\rm U{\hbox{dela}}\rm R\kern-.05em}\kern-.08em T\kern-.1667em\lower.5ex\hbox{E}\kern-.125emX\spacefactor1000}
%===========================


%==== Tolerancias ====
\tolerance=10000 % Pueden ser modificadas
\hbadness=10000 % Pueden ser modificadas
%=====================




%\hypersetup{ colorlinks = true } % false para imprimir, true para ver.
\hypersetup{ colorlinks = false } % false para imprimir, true para ver.
\usesmartdiagramlibrary{additions}
\renewcommand{\tabularxcolumn}[1]{>{\small}m{#1}}
\DeclareGraphicsRule{*}{mps}{*}{}

\newcolumntype{L}[1]{>{\raggedright\let\newline\\\arraybackslash\hspace{0pt}}m{#1}}
\newcolumntype{C}[1]{>{\centering\let\newline\\\arraybackslash\hspace{0pt}}m{#1}}
\newcolumntype{R}[1]{>{\raggedleft\let\newline\\\arraybackslash\hspace{0pt}}m{#1}}

%=============================
%==== FIN NUEVOS COMANDOS ====
%=============================

%=============================
%==== INICIO INTERLINEADO ====
%=============================

\onehalfspacing % separación entre lineas (interlineado)

\let\oldenumerate\enumerate %cambiar el entorno enumerate
\renewcommand{\enumerate}{
  \oldenumerate
  \setlength{\itemsep}{1pt} %separación entre items
  \setlength{\parskip}{0pt} 
  \setlength{\parsep}{0pt}
}

\let\olditemize\itemize %cambiar el entorno itemize
\renewcommand{\itemize}{
  \olditemize
  \setlength{\itemsep}{1pt} %separación entre items
  \setlength{\parskip}{0pt} 
  \setlength{\parsep}{0pt}
}

\let\olddescription\description %cambiar el entorno itemize
\renewcommand{\description}{
  \olddescription
  \setlength{\itemsep}{1pt} %separación entre items
  \setlength{\parskip}{0pt} 
  \setlength{\parsep}{0pt}
}

%==========================
%==== FIN INTERLINEADO ====
%==========================



%==== Opción de Maestría para la clase ===
\DeclareOption{msc}{%
  \newcommand{\@degree}{B.Sc.}
  \newcommand{\@degreename}{Grado}
  \newcommand{\local@degname}{Bachelor}
  \newcommand\local@doctype{Tesis}
}
%=========================================
%==== Opción de Examen de Doctorado para la clase ===
\DeclareOption{dscexam}{%
  \newcommand{\@degree}{D.Sc.}
  \newcommand{\@degreename}{Doctorado}
  \newcommand{\local@degname}{Doctor}
  \newcommand\local@doctype{Examen de Calificaci{\'o}n}
}
%====================================================
%==== Opción de Doctorado para la clase ===
\DeclareOption{dsc}{%
  \newcommand{\@degree}{Dr.}
  \newcommand{\@degreename}{Doctorado}
  \newcommand{\local@degname}{Doctor}
  \newcommand\local@doctype{Tesis}
}
%==========================================

%=== Comando para incluir el glosario ====
\newcommand\loadglossary{%
	\makenoidxglossaries 
	%cambio en el nombre de la lista de siglas
	\addto\captionsspanish{\renewcommand{\acronymname}{Lista de siglas}} 
	%cambio en el nombre del glosario
	\addto\captionsspanish{\renewcommand{\glossaryname}{Glosario}} 
	\newglossary*{simbolos}{Lista de símbolos} 
	\newglossary*{notacion}{Lista de notaciones}
	\newglossary*{glosario}{Glosario}
	\loadglsentries{glosario_listas_indices/simbolos}
	\loadglsentries{glosario_listas_indices/siglas}
	\loadglsentries{glosario_listas_indices/notacion}
	\loadglsentries{glosario_listas_indices/glosario}
}
%=======================================

%==== Titulo y subtitulo ====
\renewcommand\title[1]{%
  \global\def\local@title{#1}%
}
\newcount\@subtitle\@subtitle0
\newcommand\subtitle[1]{%
  \global\def\local@subtitle{#1}%
  \global\advance\@subtitle by 1
}

\newcommand\blankpage{%
    \null
    \thispagestyle{empty}%
    \addtocounter{page}{-1}%
    \newpage}
%============================

%==== Logo institución ====

\newcount\@institutelogo\@institutelogo0
\newcommand\institutelogo[1]{%
  \global\def\@instituteslogos{#1}
  \global\advance\@institutelogo by 1
}

%==========================

%==== Autor, director de tesis, codirectores y director académico ====
\renewcommand\author[2]{% Comando para el autor
  \global\def\@authname{#1}
  \global\def\@authsurn{#2}
}

%==== Pongo si quiero que sea un posgrado "en" algo o "de" algo ====
\newcommand\escritura[1]{% Comando para el tipo de posgrado
	\global\def\@tipoescritura{#1}
}

% Creo la opcion de tener varios directores de tesis
\newcommand\local@directorstring{Director}
\newcommand\foreign@directorstring{Director}

\newcount\@director\@director0
\newcommand\director[4]{% Comando para los directores
	\global\@namedef{UdelarDirectorTitle:\expandafter\the\@director}{#1}
	\global\@namedef{UdelarDirectorName:\expandafter\the\@director}{#2}
	\global\@namedef{UdelarDirectorSurname:\expandafter\the\@director}{#3}
	\global\@namedef{UdelarDirectorDegree:\expandafter\the\@director}{#4}
	\global\advance\@director by 1
	\ifnum\@director>1
	\renewcommand\local@directorstring{Directores}
	\renewcommand\foreign@directorstring{Directors}
	\fi
}


\newcommand\local@codirectorstring{Codirector}
\newcommand\foreign@codirectorstring{Co-advisor}

\newcount\@codirector\@codirector0
\newcommand\codirector[4]{% Comando para los codirectores
  \global\@namedef{UdelarCoAdvisorTitle:\expandafter\the\@codirector}{#1}
  \global\@namedef{UdelarCoAdvisorName:\expandafter\the\@codirector}{#2}
  \global\@namedef{UdelarCoAdvisorSurname:\expandafter\the\@codirector}{#3}
  \global\@namedef{UdelarCoAdvisorDegree:\expandafter\the\@codirector}{#4}
  \global\advance\@codirector by 1
  \ifnum\@codirector>1
    \renewcommand\local@codirectorstring{Codirectores}
    \renewcommand\foreign@codirectorstring{Co-dvisors}
  \fi
}

\newcount\@directoracademico\@directoracademico0
\newcommand\directoracademico[4]{% Comando para el director académico
  \global\def\@academicotitle{#1}
  \global\def\@academiconame{#2}
  \global\def\@academicosurn{#3}
  \global\def\@academicodegree{#4}
  \global\advance\@directoracademico by 1
}
%=====================================================================

%==== Examinadores ====
\newcount\@examiner\@examiner0
\newcommand\examiner[4]{% Acá se definen los examinadores del tribunal
  \global\@namedef{UdelarExaminerTitle:\expandafter\the\@examiner}{#1}
    \global\@namedef{UdelarExaminerName:\expandafter\the\@examiner}{#2}
    \global\@namedef{UdelarExaminerSurname:\expandafter\the\@examiner}{#3}
    \global\@namedef{UdelarExaminerDegree:\expandafter\the\@examiner}{#4}
  \global\advance\@examiner by 1
}
%======================

%==== Variables del posgrado ====
\ProcessOptions\relax
\newcommand\graduatename[1]{% Nombre del posgrado
  \global\def\local@graduatename{#1}
}

\newcount\@institute\@institute0
\newcounter{institute}
\newcommand\institute[2]{% Acá se definen los institutos del posgrado
  \global\@namedef{InstituteName:\expandafter\the\@institute}{#1}
  \global\@namedef{AbbrevInstituteName:\expandafter\the\@institute}{#2}
  \global\advance\@institute by 1
  \global\addtocounter{institute}{1}
}

\count1=0
\global\def\local@maininstitute{\csname InstituteName:\the\count1 \endcsname}
\global\def\local@abbrevmaininstitute{\csname AbbrevInstituteName:\the\count1\endcsname}

\newcount\@seconduniversity\@seconduniversity0
\newcounter{seconduniversity}
\newcommand\seconduniversity[1]{% Acá se definen los institutos del posgrado
	\global\@namedef{SecondUniversityName:\expandafter\the\@seconduniversity}{#1}
	\global\advance\@seconduniversity by 1
	\global\addtocounter{seconduniversity}{1}
}

\newcount\@thirduniversity\@thirduniversity0
\newcounter{thirduniversity}
\newcommand\thirduniversity[1]{% Acá se definen los institutos del posgrado
	\global\@namedef{ThirdUniversityName:\expandafter\the\@thirduniversity}{#1}
	\global\advance\@thirduniversity by 1
	\global\addtocounter{thirduniversity}{1}
}


\newcommand\graduatelocation[2]{% Nombre del posgrado
  \global\def\local@cityname{#1}
  \global\def\local@countryname{#2}
}

%================================

%=== Fecha ====
\renewcommand\date[3]{% Fecha
  \day=#1
  \month=#2
  \year=#3
}
\newcommand\local@monthname{\ifcase\month\or
  Enero\or Febrero\or Marzo\or Abril\or Mayo\or Junio\or
  Julio\or Agosto\or Setiembre\or Octubre\or Noviembre\or Diciembre\fi}
\newcommand\foreign@monthname{\ifcase\month\or
  January\or February\or March\or April\or May\or June\or
  July\or August\or September\or October\or November\or December\fi}
%==============

%==== Palabras claves ====
\newcounter{keywords}
\newcommand\keyword[1]{% Palabras claves en español
  \global\@namedef{UdelarKeyword:\expandafter\the\c@keywords}{#1}
  \global\addtocounter{keywords}{1}
}
%
\newcounter{foreignkeywords}
\newcommand\foreignkeyword[1]{% Palabras claves en inglés
  \global\@namedef{UdelarForeignKeyword:\expandafter\the\c@foreignkeywords}{#1}
  \global\addtocounter{foreignkeywords}{1}
}

%=========================

%==== Página I ====
\renewcommand\maketitle{%
  \ifthenelse{\boolean{watermark}}{\newwatermark[allpages,color=gray!50,angle=35,scale=3,xpos=0,ypos=0]{En revisión}}{}
  \begin{titlepage}
  \pagenumbering{roman}
  \pagestyle{empty}
  \setcounter{page}{1}
  \hspace{-0.5cm} \includegraphics[height=0.08\textheight]{assets/logo_udelar} \hfill
  \ifnum\@institutelogo>0
	  \ifnum\@instituteslogos>2
		  \includegraphics[height=0.08\textheight]{assets/logo_inco.pdf}  \hfil
		  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}  \hfil
		  \includegraphics[height=0.08\textheight]{assets/logo_institucion3}
	  \else
		  \ifnum\@instituteslogos>1
			  \includegraphics[height=0.08\textheight]{assets/logo_inco.pdf}  \hfil
			  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}
		  \else
		      \ifnum\@instituteslogos>0
			  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}
			  \else
			  %
			  \fi
		  \fi
      \fi
  \fi	
	  \vspace{1.05cm}
	  \begin{center}
	      \vspace*{3cm}
	      \doublespacing
		  \nohyphens{\huge{\local@title}}\par
		  \vspace*{0.3cm}
		  \ifnum\@subtitle>0
			  \nohyphens{\Large{\local@subtitle}}\par
		  \fi
		  \vspace*{2cm}
		  \nohyphens{\large{\@authname\ \@authsurn}}\par
	  \end{center}
	  \vspace{\fill}
	  \begin{center}
	  %Programa de Posgrado \@tipoescritura~\local@graduatename\\
	  \local@graduatename\\
	  \count1=0
	  \count2=1
 	  \nohyphens{\@whilenum \count1<\@institute \do {
 	  \ifnum\count2<\value{institute}
	 		\csname InstituteName:\the\count1\endcsname,
 	  \else
	 		\csname InstituteName:\the\count1\endcsname\\
 	  \fi
      \advance\count1 by 1
 	  \advance\count2 by 1}}
	\ifnum\@seconduniversity>0
		\ifnum\@thirduniversity>0
			Universidad de la Rep{\'u}blica - \csname SecondUniversityName:\the\count1\endcsname - \csname ThirdUniversityName:\the\count1\endcsname \\
		\else
			Universidad de la Rep{\'u}blica - \csname SecondUniversityName:\the\count1\endcsname \\
		\fi
	\else 
		\ifnum \@thirduniversity>0
			Universidad de la Rep{\'u}blica - \csname ThirdUniversityName:\the\count1\endcsname \\
		\else
			Universidad de la Rep{\'u}blica  \\
	    \fi
	\fi
	
	  \frontpage@bottomtext
	  \end{center}
  \end{titlepage}
  \global\let\maketitle\relax%
\global\let\and\relax}
%====================

%==== Páginas II, III y IV ====
\renewcommand\frontmatter{%
\thispagestyle{empty}
  \@mainmatterfalse
  \pagenumbering{roman}
  \setcounter{page}{2}
  \makefrontpage
}

\newcommand\makefrontpage{%
\cleardoublepage
\pagestyle{empty}
  \hspace{-0.5cm} \includegraphics[height=0.08\textheight]{assets/logo_udelar} \hfill
  \ifnum\@institutelogo>0
	  \ifnum\@instituteslogos>2
		  \includegraphics[height=0.08\textheight]{assets/logo_inco.pdf}  \hfil
		  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}  \hfil
		  \includegraphics[height=0.08\textheight]{assets/logo_institucion3}
	  \else
		  \ifnum\@instituteslogos>1
			  \includegraphics[height=0.08\textheight]{assets/logo_inco.pdf}  \hfil
			  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}
		  \else
			  \ifnum\@instituteslogos>0
				  \includegraphics[height=0.08\textheight]{assets/logo_institucion1}
			  \else
				  %
			  \fi
		  \fi
	  \fi
  \fi
\vspace{1.05cm}
\begin{center}
     \doublespacing
	 \nohyphens{\huge{\local@title}}\par
	 \vspace*{0.3cm}
	 \ifnum\@subtitle>0
	 \nohyphens{\Large{\local@subtitle}}\par
	 \fi
	 \vspace*{2cm}
	 \nohyphens{\large{\@authname\ \@authsurn}}\par
\end{center}
\vspace*{1cm}
\begin{flushright}
	\begin{minipage}{9.5cm}
		\frontcover@maintext
		
		\vspace{7.5mm}
%		Director de tesis: \\
%		\hspace*{0.3cm}\nohyphens{\@directordegree\ \@directortitle\ \@directorname\ \@directorsurn}
%		\vspace{5mm}
		%
		\ifnum\@director>0
			\local@directorstring: \\
			\count1=0
			\nohyphens{\@whilenum \count1<\@director \do {
			\hspace*{0.3cm}\csname UdelarDirectorDegree:\the\count1 \endcsname\space\csname UdelarDirectorTitle:\the\count1 \endcsname\space\csname UdelarDirectorName:\the\count1 \endcsname\space\csname UdelarDirectorSurname:\the\count1 \endcsname
			\\
			\advance\count1 by 1}}
		\fi
		
		\ifnum\@codirector>0
			\local@codirectorstring: \\
			\count1=0
			\nohyphens{\@whilenum \count1<\@codirector \do {
			\hspace*{0.3cm}\csname UdelarCoAdvisorDegree:\the\count1 \endcsname\space\csname UdelarCoAdvisorTitle:\the\count1 \endcsname\space\csname UdelarCoAdvisorName:\the\count1 \endcsname\space\csname UdelarCoAdvisorSurname:\the\count1 \endcsname
			\\
			\advance\count1 by 1}}
		\fi
		
		\ifnum\@directoracademico>0
			Director académico: \\
			\hspace*{0.3cm}\nohyphens{\@academicodegree\ \@academicotitle\ \@academiconame\ \@academicosurn}
		\fi
	\end{minipage}\par
\end{flushright}
\vspace*{\fill}	  
}


%==== Parte de la página II y III ====
\newcommand\frontcover@maintext{
  \sloppy\nohyphens{\local@doctype\ de grado
  presentada en la \local@maininstitute\ de la Universidad de la Rep{\'u}blica, como parte de los requisitos
  necesarios para la obtenci{\'o}n del t{\'i}tulo de
 grado en \local@graduatename.}
}

\newcommand\frontpage@bottomtext{%
  \begin{center}
  {\local@cityname\ -- \local@countryname}\par
  {\local@monthname\ de \number\year}
  \end{center}%
}
%=====================================

%==== Página V, puede ir o no ====
\newcommand\dedication[1]{
  \gdef\@dedic{#1}
    \cleardoublepage
    \vspace*{\fill}
    \begin{flushright}
      \begin{minipage}{60mm}
        \raggedleft  \normalsize \@dedic
      \end{minipage}
    \end{flushright}
}
%=================================

%==== Página VII, puede ir o no ====
\newcommand\epigrafe[2]{
  \gdef\@epig{#1}
  \gdef\@epigs{#2}
    \cleardoublepage
    \begin{flushright}
      \begin{minipage}{60mm}
      \begin{flushright}
        \textit{\@epig} 
        \vspace{0.5cm}
        
        \@epigs
        \end{flushright}
      \end{minipage}
    \end{flushright}
}
%===================================

%==== Página VIII, Resumen en español ====
\newenvironment{abstract}{%
  \cleardoublepage
  \thispagestyle{plain}
  \begin{center}
  	RESUMEN
  \end{center}
  \vspace*{20mm}
  }{
\vspace*{20mm}
\ifnum\value{keywords}>1
\noindent
Palabras claves:\\ 
\else
\noindent
Palabra clave:\\
\fi
\count1=0
\count2=1
\nohyphens{\@whilenum \count1<\value{keywords} \do {%
\ifnum\count2<4
	\ifnum\count2<\value{keywords}
	\csname UdelarKeyword:\the\count1 \endcsname,
	\else
	\csname UdelarKeyword:\the\count1 \endcsname.
	\fi
\else
	\ifnum\count2<\value{keywords}
	\csname UdelarKeyword:\the\count1 \endcsname,
	\else
	\csname UdelarKeyword:\the\count1 \endcsname.
	\fi
\fi
\advance\count1 by 1
\advance\count2 by 1}}
\vspace*{\fill}}

%==== Página X, lista de figuras, puede ir o no ====
\renewcommand\listoffigures{%
	\if@twoside
	  \cleardoublepage
	  \fi
	\udelar@hasLof
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listfigurename}%
      \addcontentsline{toc}{chapter}{\listfigurename}%
      \@mkboth{\MakeUppercase\listfigurename}%
              {\MakeUppercase\listfigurename}%
    \@starttoc{lof}%
    \if@restonecol\twocolumn\fi
}
%===================================================

%==== Página XI, lista de tablas, puede ir o no ====    
\renewcommand\listoftables{%
    \if@twocolumn
      \@restonecoltrue\onecolumn
    \else
      \@restonecolfalse
    \fi
    \chapter*{\listtablename}%
      \addcontentsline{toc}{chapter}{\listtablename}%
      \@mkboth{%
          \MakeUppercase\listtablename}%
         {\MakeUppercase\listtablename}%
    \@starttoc{lot}%
    \if@restonecol\twocolumn\fi
}
%===================================================

%==== Lista de símbolos, Página XII, puede ir o no ====
\newcommand\listadesimbolos{%
	\printnoidxglossary[sort=case,type=simbolos]\addcontentsline{toc}{chapter}{Lista de símbolos}
}
%======================================================

%==== Lista de acrónimos, Página XIII, puede ir o no ====
\newcommand\listadesiglas{%
	\printnoidxglossary[sort=case,type=acronym]\addcontentsline{toc}{chapter}{Lista de siglas}
}
%========================================================

%==== Lista de notación, Página XIV, puede ir o no ====
\newcommand\listadenotaciones{%
	\printnoidxglossary[sort=case,type=notacion]\addcontentsline{toc}{chapter}{Notaciones}
}
%======================================================

%==== Contenido general del texto ===
\renewcommand\mainmatter{%
  \ifthenelse{\boolean{linenumbers}}{\linenumbers}{\nolinenumbers}
  \if@twoside
  \clearpage
  \fi
  \udelar@mainBegin
  \cleardoublepage
  \@mainmattertrue
  \pagestyle{plain}
  \pagenumbering{arabic}
}
%====================================

%==== Final del texto ====
\renewcommand\backmatter{%
  \if@openright
    \cleardoublepage
  \else
    \clearpage
    
\fi
\udelar@bibBegin}
%=========================
    
%==== Algunos labels importantes ====
\AtBeginDocument{%
  \ltx@ifpackageloaded{hyperref}{
  	\def\udelar@bibBegin{%
        \immediate\write\@auxout{%
          \string\newlabel{bib:begin}{{}{\arabic{page}}{}{page.\arabic{page}}{}}}}%
    \def\udelar@bibEnd{%
      \immediate\write\@auxout{%
        \string\newlabel{bib:end}{{}{\arabic{page}}{}{page.\arabic{page}}{}}}}%
    \def\udelar@mainBegin{%
          \immediate\write\@auxout{%
            \string\newlabel{front:pageno}{{}{\Roman{page}}{}{page.\Roman{page}}{}}}}%
    \def\udelar@hasLof{%
      \immediate\write\@auxout{%
        \string\newlabel{cat:lofflag}{{}{:~il.;}{}{page.\Roman{page}}{}}}}%
  }{%
    \def\udelar@bibEnd{%
      \immediate\write\@auxout{%
      \string\newlabel{bib:end}{{}{\arabic{page}}{}}}}%
    \def\udelar@bibBegin{%
      \immediate\write\@auxout{%
      \string\newlabel{bib:begin}{{}{\arabic{page}}{}}}}%
    \def\udelar@mainBegin{%
      \immediate\write\@auxout{%
        \string\newlabel{front:pageno}{{}{\Roman{page}}{}}}}%
    \def\udelar@hasLof{%
      \immediate\write\@auxout{%
        \string\newlabel{cat:lofflag}{{}{:~il.;}{}}}}%
  }%
}
%====================================

%==== Bibliografía ====


\newdimen\bibindent%
\setlength\bibindent{1.5em}%
\renewenvironment{thebibliography}[1]%
     {\onehalfspacing%
      \chapter*{\bibname}%
      \addcontentsline{toc}{chapter}{\bibname}%
      \list{\@biblabel{\@arabic\c@enumiv}}%
           {\setlength{\labelwidth}{0ex}%
            \setlength{\leftmargin}{0.0ex}%
            \setlength{\itemindent}{-2.5ex}%
            \advance\leftmargin\labelsep%
            \@openbib@code%
            \usecounter{enumiv}%
            \let\p@enumiv\@empty%
            \renewcommand\theenumiv{\@arabic\c@enumiv}}%
      \sloppy%
      \clubpenalty4000%
      \@clubpenalty \clubpenalty%
      \widowpenalty4000%
      \sfcode`\.\@m}%
     {\def\@noitemerr%
       {\@latex@warning{Empty `thebibliography' environment}}%
\endlist}
\newcommand\bibend{\udelar@bibEnd}
%======================

%==== Glosario ====
\newcommand\glosario{%
	\printnoidxglossary[sort=case,type=glosario]\addcontentsline{toc}{chapter}{Glosario} 
}
%==================

\newcount\@apenarabicnumbering\@apenarabicnumbering0
\newcommand\apenarabicnumbering{%
  \global\advance\@apenarabicnumbering by 1
}

\newcount\@anexarabicnumbering\@anexarabicnumbering0
\newcommand\anexarabicnumbering{%
  \global\advance\@anexarabicnumbering by 1
}

%==== Apéndices ====
\newcommand\apenmatter{%
	\appendix
	\renewcommand\appendixpagename{APÉNDICES}
	\renewcommand\appendixtocname{\textbf{Apéndices}}
	\appendixpage
	\ifnum\@apenarabicnumbering>0
	\renewcommand{\thechapter}{\arabic{chapter}}
	\fi
	\titlecontents{chapter}% <section-type>
		[13pt]% <left>
		{}% <above-code>
		{ Apéndice\ \thecontentslabel\quad}% <numbered-entry-format>
		{}% <numberless-entry-format>
		{\titlerule*[1pc]{.}\contentspage}% <filler-page-format>
}


%===================

%==== Anexos ====
\newcommand\anexmatter{%
\newcounter{myc}
\setcounter{myc}{1}
	\appendix
	\renewcommand\appendixpagename{ANEXOS}
	\renewcommand\appendixtocname{\textbf{Anexos}}
	\renewcommand\appendixname{Anexo}
	\titlecontents{chapter}% <section-type>
			[0pt]% <left>
			{\vspace*{0.45cm}}% <above-code>
			{\thecontentslabel\quad}% <numbered-entry-format>
			{}% <numberless-entry-format>
			{\bfseries\hfill\contentspage}% <filler-page-format>
	\appendixpage
	\ifnum\@anexarabicnumbering>0
		\renewcommand{\thechapter}{\arabic{myc}}
		\else
		\renewcommand{\thechapter}{\Alph{myc}}
		\fi
	\titlecontents{chapter}% <section-type>
			[13pt]% <left>
			{}% <above-code>
			{ Anexo\ \thecontentslabel\quad}% <numbered-entry-format>
			{}% <numberless-entry-format>
			{\titlerule*[1pc]{.}\contentspage}% <filler-page-format>
			\setcounter{chapter}{-1}
}
%================

%================

%==== CUSTOM ====

\newcommand{\Figref}[1]{Figura~\ref{#1}}
\newcommand{\ipc}{{\sf ipc}}

\newcommand{\Prob}{\bbbp}
\newcommand{\Real}{\bbbr}
\newcommand{\real}{\Real}
\newcommand{\Int}{\bbbz}
\newcommand{\Nat}{\bbbn}

\newcommand{\NN}{{\sf I\kern-0.14emN}}   % Natural numbers
\newcommand{\ZZ}{{\sf Z\kern-0.45emZ}}   % Integers
\newcommand{\QQQ}{{\sf C\kern-0.48emQ}}   % Rational numbers
\newcommand{\RR}{{\sf I\kern-0.14emR}}   % Real numbers
\newcommand{\KK}{{\cal K}}
\newcommand{\OO}{{\cal O}}
\newcommand{\AAA}{{\bf A}}
\newcommand{\HH}{{\bf H}}
\newcommand{\II}{{\bf I}}
\newcommand{\LL}{{\bf L}}
\newcommand{\PP}{{\bf P}}
\newcommand{\PPprime}{{\bf P'}}
\newcommand{\QQ}{{\bf Q}}
\newcommand{\UU}{{\bf U}}
\newcommand{\UUprime}{{\bf U'}}
\newcommand{\zzero}{{\bf 0}}
\newcommand{\ppi}{\mbox{\boldmath $\pi$}}
\newcommand{\aalph}{\mbox{\boldmath $\alpha$}}
\newcommand{\bb}{{\bf b}}
\newcommand{\ee}{{\bf e}}
\newcommand{\mmu}{\mbox{\boldmath $\mu$}}
\newcommand{\vv}{{\bf v}}
\newcommand{\xx}{{\bf x}}
\newcommand{\yy}{{\bf y}}
\newcommand{\zz}{{\bf z}}
\newcommand{\oomeg}{\mbox{\boldmath $\omega$}}
\newcommand{\res}{{\bf res}}
\newcommand{\cchi}{{\mbox{\raisebox{.4ex}{$\chi$}}}}
%\newcommand{\cchi}{{\cal X}}
%\newcommand{\cchi}{\mbox{\Large $\chi$}}

% Logical operators and symbols
\newcommand{\imply}{\Rightarrow}
\newcommand{\bimply}{\Leftrightarrow}
\newcommand{\union}{\cup}
\newcommand{\intersect}{\cap}
\newcommand{\boolor}{\vee}
\newcommand{\booland}{\wedge}
\newcommand{\boolimply}{\imply}
\newcommand{\boolbimply}{\bimply}
\newcommand{\boolnot}{\neg}
\newcommand{\boolsat}{\!\models}
\newcommand{\boolnsat}{\!\not\models}


\newcommand{\op}[1]{\mathrm{#1}}
\newcommand{\s}[1]{\ensuremath{\mathcal #1}}

% Properly styled differentiation and integration operators
\newcommand{\diff}[1]{\mathrm{\frac{d}{d\mathit{#1}}}}
\newcommand{\diffII}[1]{\mathrm{\frac{d^2}{d\mathit{#1}^2}}}
\newcommand{\intg}[4]{\int_{#3}^{#4} #1 \, \mathrm{d}#2}
\newcommand{\intgd}[4]{\int\!\!\!\!\int_{#4} #1 \, \mathrm{d}#2 \, \mathrm{d}#3}

% Large () brackets on different lines of an eqnarray environment
\newcommand{\Leftbrace}[1]{\left(\raisebox{0mm}[#1][#1]{}\right.}
\newcommand{\Rightbrace}[1]{\left.\raisebox{0mm}[#1][#1]{}\right)}

% Funky symobols for footnotes
\newcommand{\symbolfootnote}{\renewcommand{\thefootnote}{\fnsymbol{footnote}}}
% now add \symbolfootnote to the beginning of the document...

\newcommand{\normallinespacing}{\renewcommand{\baselinestretch}{1.5} \normalsize}
\newcommand{\mediumlinespacing}{\renewcommand{\baselinestretch}{1.2} \normalsize}
\newcommand{\narrowlinespacing}{\renewcommand{\baselinestretch}{1.0} \normalsize}
\newcommand{\bump}{\noalign{\vspace*{\doublerulesep}}}
\newcommand{\cell}{\multicolumn{1}{}{}}
\newcommand{\spann}{\mbox{span}}
\newcommand{\diagg}{\mbox{diag}}
\newcommand{\modd}{\mbox{mod}}
\newcommand{\minn}{\mbox{min}}
\newcommand{\andd}{\mbox{and}}
\newcommand{\forr}{\mbox{for}}
\newcommand{\EE}{\mbox{E}}

\newcommand{\deff}{\stackrel{\mathrm{def}}{=}}
\newcommand{\syncc}{~\stackrel{\textstyle \rhd\kern-0.57em\lhd}{\scriptstyle L}~}




%% 
%%
%% End of file `udelar.cls'.