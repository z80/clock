




<!DOCTYPE html>
<html class="  ">
  <head prefix="og: http://ogp.me/ns# fb: http://ogp.me/ns/fb# object: http://ogp.me/ns/object# article: http://ogp.me/ns/article# profile: http://ogp.me/ns/profile#">
    <meta charset='utf-8'>
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    
    
    <title>ChibiOS-RT/os/various/devices_lib/lcd/lcd3310.h at master · ChibiOS/ChibiOS-RT</title>
    <link rel="search" type="application/opensearchdescription+xml" href="/opensearch.xml" title="GitHub" />
    <link rel="fluid-icon" href="https://github.com/fluidicon.png" title="GitHub" />
    <link rel="apple-touch-icon" sizes="57x57" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="114x114" href="/apple-touch-icon-114.png" />
    <link rel="apple-touch-icon" sizes="72x72" href="/apple-touch-icon-144.png" />
    <link rel="apple-touch-icon" sizes="144x144" href="/apple-touch-icon-144.png" />
    <meta property="fb:app_id" content="1401488693436528"/>

      <meta content="@github" name="twitter:site" /><meta content="summary" name="twitter:card" /><meta content="ChibiOS/ChibiOS-RT" name="twitter:title" /><meta content="ChibiOS-RT - Mirror of the ChibiOS/RT SVN Repo (stable and trunk branches only)" name="twitter:description" /><meta content="https://avatars1.githubusercontent.com/u/2803041?s=400" name="twitter:image:src" />
<meta content="GitHub" property="og:site_name" /><meta content="object" property="og:type" /><meta content="https://avatars1.githubusercontent.com/u/2803041?s=400" property="og:image" /><meta content="ChibiOS/ChibiOS-RT" property="og:title" /><meta content="https://github.com/ChibiOS/ChibiOS-RT" property="og:url" /><meta content="ChibiOS-RT - Mirror of the ChibiOS/RT SVN Repo (stable and trunk branches only)" property="og:description" />

    <link rel="assets" href="https://github.global.ssl.fastly.net/">
    <link rel="conduit-xhr" href="https://ghconduit.com:25035/">
    <link rel="xhr-socket" href="/_sockets" />

    <meta name="msapplication-TileImage" content="/windows-tile.png" />
    <meta name="msapplication-TileColor" content="#ffffff" />
    <meta name="selected-link" value="repo_source" data-pjax-transient />
    <meta content="collector.githubapp.com" name="octolytics-host" /><meta content="collector-cdn.github.com" name="octolytics-script-host" /><meta content="github" name="octolytics-app-id" /><meta content="A2CD0296:5AE0:1ECD613:53339EE4" name="octolytics-dimension-request_id" /><meta content="1449921" name="octolytics-actor-id" /><meta content="z80" name="octolytics-actor-login" /><meta content="f06d360518ab1e245520b51a46c092ec16f3c4fc530a3dbea216e3e097d96838" name="octolytics-actor-hash" />
    

    
    
    <link rel="icon" type="image/x-icon" href="https://github.global.ssl.fastly.net/favicon.ico" />

    <meta content="authenticity_token" name="csrf-param" />
<meta content="0fagTVlTBSEwyS/rzzOownomqvMzPxGkDvIUehVUxe8=" name="csrf-token" />

    <link href="https://github.global.ssl.fastly.net/assets/github-87b6408dab1c2b10a501b0c95893820bd43f5586.css" media="all" rel="stylesheet" type="text/css" />
    <link href="https://github.global.ssl.fastly.net/assets/github2-ca90759af59cbf1dab8f53d694561f34910460cc.css" media="all" rel="stylesheet" type="text/css" />
    


        <script crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/frameworks-6d4c8306a4ec71dc768c21d5f3121181d8582b5f.js" type="text/javascript"></script>
        <script async="async" crossorigin="anonymous" src="https://github.global.ssl.fastly.net/assets/github-ee0fbfda7236e547fc0a4233c85857f84e7cf76b.js" type="text/javascript"></script>
        
        
      <meta http-equiv="x-pjax-version" content="0cfa79ead76b37004085658bd3a18a09">

        <link data-pjax-transient rel='permalink' href='/ChibiOS/ChibiOS-RT/blob/ecf94119561c3eeaa8d1ab35b76c19228a1d362e/os/various/devices_lib/lcd/lcd3310.h'>

  <meta name="description" content="ChibiOS-RT - Mirror of the ChibiOS/RT SVN Repo (stable and trunk branches only)" />

  <meta content="2803041" name="octolytics-dimension-user_id" /><meta content="ChibiOS" name="octolytics-dimension-user_login" /><meta content="6637893" name="octolytics-dimension-repository_id" /><meta content="ChibiOS/ChibiOS-RT" name="octolytics-dimension-repository_nwo" /><meta content="true" name="octolytics-dimension-repository_public" /><meta content="false" name="octolytics-dimension-repository_is_fork" /><meta content="6637893" name="octolytics-dimension-repository_network_root_id" /><meta content="ChibiOS/ChibiOS-RT" name="octolytics-dimension-repository_network_root_nwo" />
  <link href="https://github.com/ChibiOS/ChibiOS-RT/commits/master.atom" rel="alternate" title="Recent Commits to ChibiOS-RT:master" type="application/atom+xml" />

  </head>


  <body class="logged_in  env-production windows vis-public page-blob">
    <a href="#start-of-content" class="accessibility-aid js-skip-to-content">Skip to content</a>
    <div class="wrapper">
      
      
      
      


      <div class="header header-logged-in true">
  <div class="container clearfix">

    <a class="header-logo-invertocat" href="https://github.com/">
  <span class="mega-octicon octicon-mark-github"></span>
</a>

    
    <a href="/notifications" aria-label="You have no unread notifications" class="notification-indicator tooltipped tooltipped-s" data-gotokey="n">
        <span class="mail-status all-read"></span>
</a>

      <div class="command-bar js-command-bar  in-repository">
          <form accept-charset="UTF-8" action="/search" class="command-bar-form" id="top_search_form" method="get">

<div class="commandbar">
  <span class="message"></span>
  <input type="text" data-hotkey="/ s" name="q" id="js-command-bar-field" placeholder="Search or type a command" tabindex="1" autocapitalize="off"
    
    data-username="z80"
      data-repo="ChibiOS/ChibiOS-RT"
      data-branch="master"
      data-sha="abafcbdbad1cdd9232242913de8c422998cd5e67"
  >
  <div class="display hidden"></div>
</div>

    <input type="hidden" name="nwo" value="ChibiOS/ChibiOS-RT" />

    <div class="select-menu js-menu-container js-select-menu search-context-select-menu">
      <span class="minibutton select-menu-button js-menu-target" role="button" aria-haspopup="true">
        <span class="js-select-button">This repository</span>
      </span>

      <div class="select-menu-modal-holder js-menu-content js-navigation-container" aria-hidden="true">
        <div class="select-menu-modal">

          <div class="select-menu-item js-navigation-item js-this-repository-navigation-item selected">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" class="js-search-this-repository" name="search_target" value="repository" checked="checked" />
            <div class="select-menu-item-text js-select-button-text">This repository</div>
          </div> <!-- /.select-menu-item -->

          <div class="select-menu-item js-navigation-item js-all-repositories-navigation-item">
            <span class="select-menu-item-icon octicon octicon-check"></span>
            <input type="radio" name="search_target" value="global" />
            <div class="select-menu-item-text js-select-button-text">All repositories</div>
          </div> <!-- /.select-menu-item -->

        </div>
      </div>
    </div>

  <span class="help tooltipped tooltipped-s" aria-label="Show command bar help">
    <span class="octicon octicon-question"></span>
  </span>


  <input type="hidden" name="ref" value="cmdform">

</form>
        <ul class="top-nav">
          <li class="explore"><a href="/explore">Explore</a></li>
            <li><a href="https://gist.github.com">Gist</a></li>
            <li><a href="/blog">Blog</a></li>
          <li><a href="https://help.github.com">Help</a></li>
        </ul>
      </div>

    


  <ul id="user-links">
    <li>
      <a href="/z80" class="name">
        <img alt="z80" class=" js-avatar" data-user="1449921" height="20" src="https://avatars2.githubusercontent.com/u/1449921?s=140" width="20" /> z80
      </a>
    </li>

    <li class="new-menu dropdown-toggle js-menu-container">
      <a href="#" class="js-menu-target tooltipped tooltipped-s" aria-label="Create new...">
        <span class="octicon octicon-plus"></span>
        <span class="dropdown-arrow"></span>
      </a>

      <div class="new-menu-content js-menu-content">
      </div>
    </li>

    <li>
      <a href="/settings/profile" id="account_settings"
        class="tooltipped tooltipped-s"
        aria-label="Account settings ">
        <span class="octicon octicon-tools"></span>
      </a>
    </li>
    <li>
      <a class="tooltipped tooltipped-s" href="/logout" data-method="post" id="logout" aria-label="Sign out">
        <span class="octicon octicon-log-out"></span>
      </a>
    </li>

  </ul>

<div class="js-new-dropdown-contents hidden">
  

<ul class="dropdown-menu">
  <li>
    <a href="/new"><span class="octicon octicon-repo-create"></span> New repository</a>
  </li>
  <li>
    <a href="/organizations/new"><span class="octicon octicon-organization"></span> New organization</a>
  </li>


</ul>

</div>


    
  </div>
</div>

      

        



      <div id="start-of-content" class="accessibility-aid"></div>
          <div class="site" itemscope itemtype="http://schema.org/WebPage">
    
    <div class="pagehead repohead instapaper_ignore readability-menu">
      <div class="container">
        

<ul class="pagehead-actions">

    <li class="subscription">
      <form accept-charset="UTF-8" action="/notifications/subscribe" class="js-social-container" data-autosubmit="true" data-remote="true" method="post"><div style="margin:0;padding:0;display:inline"><input name="authenticity_token" type="hidden" value="0fagTVlTBSEwyS/rzzOownomqvMzPxGkDvIUehVUxe8=" /></div>  <input id="repository_id" name="repository_id" type="hidden" value="6637893" />

    <div class="select-menu js-menu-container js-select-menu">
      <a class="social-count js-social-count" href="/ChibiOS/ChibiOS-RT/watchers">
        32
      </a>
      <span class="minibutton select-menu-button with-count js-menu-target" role="button" tabindex="0" aria-haspopup="true">
        <span class="js-select-button">
          <span class="octicon octicon-eye-watch"></span>
          Watch
        </span>
      </span>

      <div class="select-menu-modal-holder">
        <div class="select-menu-modal subscription-menu-modal js-menu-content" aria-hidden="true">
          <div class="select-menu-header">
            <span class="select-menu-title">Notification status</span>
            <span class="octicon octicon-remove-close js-menu-close"></span>
          </div> <!-- /.select-menu-header -->

          <div class="select-menu-list js-navigation-container" role="menu">

            <div class="select-menu-item js-navigation-item selected" role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input checked="checked" id="do_included" name="do" type="radio" value="included" />
                <h4>Not watching</h4>
                <span class="description">You only receive notifications for conversations in which you participate or are @mentioned.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-watch"></span>
                  Watch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item " role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_subscribed" name="do" type="radio" value="subscribed" />
                <h4>Watching</h4>
                <span class="description">You receive notifications for all conversations in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-eye-unwatch"></span>
                  Unwatch
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

            <div class="select-menu-item js-navigation-item " role="menuitem" tabindex="0">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <div class="select-menu-item-text">
                <input id="do_ignore" name="do" type="radio" value="ignore" />
                <h4>Ignoring</h4>
                <span class="description">You do not receive any notifications for conversations in this repository.</span>
                <span class="js-select-button-text hidden-select-button-text">
                  <span class="octicon octicon-mute"></span>
                  Stop ignoring
                </span>
              </div>
            </div> <!-- /.select-menu-item -->

          </div> <!-- /.select-menu-list -->

        </div> <!-- /.select-menu-modal -->
      </div> <!-- /.select-menu-modal-holder -->
    </div> <!-- /.select-menu -->

</form>
    </li>

  <li>
  

  <div class="js-toggler-container js-social-container starring-container ">
    <a href="/ChibiOS/ChibiOS-RT/unstar"
      class="minibutton with-count js-toggler-target star-button starred"
      aria-label="Unstar this repository" title="Unstar ChibiOS/ChibiOS-RT" data-remote="true" data-method="post" rel="nofollow">
      <span class="octicon octicon-star-delete"></span><span class="text">Unstar</span>
    </a>

    <a href="/ChibiOS/ChibiOS-RT/star"
      class="minibutton with-count js-toggler-target star-button unstarred"
      aria-label="Star this repository" title="Star ChibiOS/ChibiOS-RT" data-remote="true" data-method="post" rel="nofollow">
      <span class="octicon octicon-star"></span><span class="text">Star</span>
    </a>

      <a class="social-count js-social-count" href="/ChibiOS/ChibiOS-RT/stargazers">
        57
      </a>
  </div>

  </li>


        <li>
          <a href="/ChibiOS/ChibiOS-RT/fork" class="minibutton with-count js-toggler-target fork-button lighter tooltipped-n" title="Fork your own copy of ChibiOS/ChibiOS-RT to your account" aria-label="Fork your own copy of ChibiOS/ChibiOS-RT to your account" rel="nofollow" data-method="post">
            <span class="octicon octicon-git-branch-create"></span><span class="text">Fork</span>
          </a>
          <a href="/ChibiOS/ChibiOS-RT/network" class="social-count">60</a>
        </li>


</ul>

        <h1 itemscope itemtype="http://data-vocabulary.org/Breadcrumb" class="entry-title public">
          <span class="repo-label"><span>public</span></span>
          <span class="mega-octicon octicon-repo"></span>
          <span class="author">
            <a href="/ChibiOS" class="url fn" itemprop="url" rel="author"><span itemprop="title">ChibiOS</span></a>
          </span>
          <span class="repohead-name-divider">/</span>
          <strong><a href="/ChibiOS/ChibiOS-RT" class="js-current-repository js-repo-home-link">ChibiOS-RT</a></strong>

          <span class="page-context-loader">
            <img alt="Octocat-spinner-32" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
          </span>

        </h1>
      </div><!-- /.container -->
    </div><!-- /.repohead -->

    <div class="container">
      <div class="repository-with-sidebar repo-container new-discussion-timeline js-new-discussion-timeline  ">
        <div class="repository-sidebar clearfix">
            

<div class="sunken-menu vertical-right repo-nav js-repo-nav js-repository-container-pjax js-octicon-loaders">
  <div class="sunken-menu-contents">
    <ul class="sunken-menu-group">
      <li class="tooltipped tooltipped-w" aria-label="Code">
        <a href="/ChibiOS/ChibiOS-RT" aria-label="Code" class="selected js-selected-navigation-item sunken-menu-item" data-gotokey="c" data-pjax="true" data-selected-links="repo_source repo_downloads repo_commits repo_tags repo_branches /ChibiOS/ChibiOS-RT">
          <span class="octicon octicon-code"></span> <span class="full-word">Code</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


      <li class="tooltipped tooltipped-w" aria-label="Pull Requests">
        <a href="/ChibiOS/ChibiOS-RT/pulls" aria-label="Pull Requests" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-gotokey="p" data-selected-links="repo_pulls /ChibiOS/ChibiOS-RT/pulls">
            <span class="octicon octicon-git-pull-request"></span> <span class="full-word">Pull Requests</span>
            <span class='counter'>0</span>
            <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>


    </ul>
    <div class="sunken-menu-separator"></div>
    <ul class="sunken-menu-group">

      <li class="tooltipped tooltipped-w" aria-label="Pulse">
        <a href="/ChibiOS/ChibiOS-RT/pulse" aria-label="Pulse" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="pulse /ChibiOS/ChibiOS-RT/pulse">
          <span class="octicon octicon-pulse"></span> <span class="full-word">Pulse</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Graphs">
        <a href="/ChibiOS/ChibiOS-RT/graphs" aria-label="Graphs" class="js-selected-navigation-item sunken-menu-item" data-pjax="true" data-selected-links="repo_graphs repo_contributors /ChibiOS/ChibiOS-RT/graphs">
          <span class="octicon octicon-graph"></span> <span class="full-word">Graphs</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>

      <li class="tooltipped tooltipped-w" aria-label="Network">
        <a href="/ChibiOS/ChibiOS-RT/network" aria-label="Network" class="js-selected-navigation-item sunken-menu-item js-disable-pjax" data-selected-links="repo_network /ChibiOS/ChibiOS-RT/network">
          <span class="octicon octicon-git-branch"></span> <span class="full-word">Network</span>
          <img alt="Octocat-spinner-32" class="mini-loader" height="16" src="https://github.global.ssl.fastly.net/images/spinners/octocat-spinner-32.gif" width="16" />
</a>      </li>
    </ul>


  </div>
</div>

              <div class="only-with-full-nav">
                

  

<div class="clone-url open"
  data-protocol-type="http"
  data-url="/users/set_protocol?protocol_selector=http&amp;protocol_type=clone">
  <h3><strong>HTTPS</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/ChibiOS/ChibiOS-RT.git" readonly="readonly">

    <span aria-label="copy to clipboard" class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/ChibiOS/ChibiOS-RT.git" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="ssh"
  data-url="/users/set_protocol?protocol_selector=ssh&amp;protocol_type=clone">
  <h3><strong>SSH</strong> clone URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="git@github.com:ChibiOS/ChibiOS-RT.git" readonly="readonly">

    <span aria-label="copy to clipboard" class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="git@github.com:ChibiOS/ChibiOS-RT.git" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>

  

<div class="clone-url "
  data-protocol-type="subversion"
  data-url="/users/set_protocol?protocol_selector=subversion&amp;protocol_type=clone">
  <h3><strong>Subversion</strong> checkout URL</h3>
  <div class="clone-url-box">
    <input type="text" class="clone js-url-field"
           value="https://github.com/ChibiOS/ChibiOS-RT" readonly="readonly">

    <span aria-label="copy to clipboard" class="js-zeroclipboard url-box-clippy minibutton zeroclipboard-button" data-clipboard-text="https://github.com/ChibiOS/ChibiOS-RT" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


<p class="clone-options">You can clone with
      <a href="#" class="js-clone-selector" data-protocol="http">HTTPS</a>,
      <a href="#" class="js-clone-selector" data-protocol="ssh">SSH</a>,
      or <a href="#" class="js-clone-selector" data-protocol="subversion">Subversion</a>.
  <span class="help tooltipped tooltipped-n" aria-label="Get help on which URL is right for you.">
    <a href="https://help.github.com/articles/which-remote-url-should-i-use">
    <span class="octicon octicon-question"></span>
    </a>
  </span>
</p>


  <a href="http://windows.github.com" class="minibutton sidebar-button" title="Save ChibiOS/ChibiOS-RT to your computer and use it in GitHub Desktop." aria-label="Save ChibiOS/ChibiOS-RT to your computer and use it in GitHub Desktop.">
    <span class="octicon octicon-device-desktop"></span>
    Clone in Desktop
  </a>

                <a href="/ChibiOS/ChibiOS-RT/archive/master.zip"
                   class="minibutton sidebar-button"
                   aria-label="Download ChibiOS/ChibiOS-RT as a zip file"
                   title="Download ChibiOS/ChibiOS-RT as a zip file"
                   rel="nofollow">
                  <span class="octicon octicon-cloud-download"></span>
                  Download ZIP
                </a>
              </div>
        </div><!-- /.repository-sidebar -->

        <div id="js-repo-pjax-container" class="repository-content context-loader-container" data-pjax-container>
          


<!-- blob contrib key: blob_contributors:v21:03af4aa4a3ab3cb18126811574d84b9f -->

<p title="This is a placeholder element" class="js-history-link-replace hidden"></p>

<a href="/ChibiOS/ChibiOS-RT/find/master" data-pjax data-hotkey="t" class="js-show-file-finder" style="display:none">Show File Finder</a>

<div class="file-navigation">
  

<div class="select-menu js-menu-container js-select-menu" >
  <span class="minibutton select-menu-button js-menu-target" data-hotkey="w"
    data-master-branch="master"
    data-ref="master"
    role="button" aria-label="Switch branches or tags" tabindex="0" aria-haspopup="true">
    <span class="octicon octicon-git-branch"></span>
    <i>branch:</i>
    <span class="js-select-button">master</span>
  </span>

  <div class="select-menu-modal-holder js-menu-content js-navigation-container" data-pjax aria-hidden="true">

    <div class="select-menu-modal">
      <div class="select-menu-header">
        <span class="select-menu-title">Switch branches/tags</span>
        <span class="octicon octicon-remove-close js-menu-close"></span>
      </div> <!-- /.select-menu-header -->

      <div class="select-menu-filters">
        <div class="select-menu-text-filter">
          <input type="text" aria-label="Filter branches/tags" id="context-commitish-filter-field" class="js-filterable-field js-navigation-enable" placeholder="Filter branches/tags">
        </div>
        <div class="select-menu-tabs">
          <ul>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="branches" class="js-select-menu-tab">Branches</a>
            </li>
            <li class="select-menu-tab">
              <a href="#" data-tab-filter="tags" class="js-select-menu-tab">Tags</a>
            </li>
          </ul>
        </div><!-- /.select-menu-tabs -->
      </div><!-- /.select-menu-filters -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="branches">

        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/chibistudio_trunk/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="chibistudio_trunk"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="chibistudio_trunk">chibistudio_trunk</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/kernel_3_alt_vt_dev/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="kernel_3_alt_vt_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="kernel_3_alt_vt_dev">kernel_3_alt_vt_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/kernel_3_dev/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="kernel_3_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="kernel_3_dev">kernel_3_dev</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item selected">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/master/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="master"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="master">master</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_1.0.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_1.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.0.x">stable_1.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_1.2.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_1.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.2.x">stable_1.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_1.4.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_1.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_1.4.x">stable_1.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_2.0.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_2.0.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.0.x">stable_2.0.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_2.2.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_2.2.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.2.x">stable_2.2.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_2.4.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_2.4.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.4.x">stable_2.4.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/stable_2.6.x/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="stable_2.6.x"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="stable_2.6.x">stable_2.6.x</a>
            </div> <!-- /.select-menu-item -->
            <div class="select-menu-item js-navigation-item ">
              <span class="select-menu-item-icon octicon octicon-check"></span>
              <a href="/ChibiOS/ChibiOS-RT/blob/utils_dev/os/various/devices_lib/lcd/lcd3310.h"
                 data-name="utils_dev"
                 data-skip-pjax="true"
                 rel="nofollow"
                 class="js-navigation-open select-menu-item-text js-select-button-text css-truncate-target"
                 title="utils_dev">utils_dev</a>
            </div> <!-- /.select-menu-item -->
        </div>

          <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

      <div class="select-menu-list select-menu-tab-bucket js-select-menu-tab-bucket" data-tab-filter="tags">
        <div data-filterable-for="context-commitish-filter-field" data-filterable-type="substring">


        </div>

        <div class="select-menu-no-results">Nothing to show</div>
      </div> <!-- /.select-menu-list -->

    </div> <!-- /.select-menu-modal -->
  </div> <!-- /.select-menu-modal-holder -->
</div> <!-- /.select-menu -->

  <div class="breadcrumb">
    <span class='repo-root js-repo-root'><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/ChibiOS/ChibiOS-RT" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">ChibiOS-RT</span></a></span></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/ChibiOS/ChibiOS-RT/tree/master/os" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">os</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/ChibiOS/ChibiOS-RT/tree/master/os/various" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">various</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/ChibiOS/ChibiOS-RT/tree/master/os/various/devices_lib" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">devices_lib</span></a></span><span class="separator"> / </span><span itemscope="" itemtype="http://data-vocabulary.org/Breadcrumb"><a href="/ChibiOS/ChibiOS-RT/tree/master/os/various/devices_lib/lcd" data-branch="master" data-direction="back" data-pjax="true" itemscope="url"><span itemprop="title">lcd</span></a></span><span class="separator"> / </span><strong class="final-path">lcd3310.h</strong> <span aria-label="copy to clipboard" class="js-zeroclipboard minibutton zeroclipboard-button" data-clipboard-text="os/various/devices_lib/lcd/lcd3310.h" data-copied-hint="copied!"><span class="octicon octicon-clippy"></span></span>
  </div>
</div>


  <div class="commit file-history-tease">
    <img class="main-avatar" height="24" src="https://0.gravatar.com/avatar/5326dd1d3af5083e50cded25201e29d4?d=https%3A%2F%2Fa248.e.akamai.net%2Fassets.github.com%2Fimages%2Fgravatars%2Fgravatar-user-420.png&amp;r=x&amp;s=140" width="24" />
    <span class="author"><span rel="author">theshed</span></span>
    <time class="js-relative-date" data-title-format="YYYY-MM-DD HH:mm:ss" datetime="2013-06-01T22:00:28Z" title="2013-06-01 22:00:28">June 01, 2013</time>
    <div class="commit-title">
        <a href="/ChibiOS/ChibiOS-RT/commit/4279bb0b509dff69d91d7bfd7e4f9c5d439ac361" class="message" data-pjax="true" title="More LPC122x drivers from Marcin J.

git-svn-id: svn://svn.code.sf.net/p/chibios/svn/trunk@5794 35acf78f-673a-0410-8e92-d51de3d6d3f4">More LPC122x drivers from Marcin J.</a>
    </div>

    <div class="participation">
      <p class="quickstat"><a href="#blob_contributors_box" rel="facebox"><strong>0</strong> contributors</a></p>
      
    </div>
    <div id="blob_contributors_box" style="display:none">
      <h2 class="facebox-header">Users who have contributed to this file</h2>
      <ul class="facebox-user-list">
      </ul>
    </div>
  </div>

<div class="file-box">
  <div class="file">
    <div class="meta clearfix">
      <div class="info file-name">
        <span class="icon"><b class="octicon octicon-file-text"></b></span>
        <span class="mode" title="File Mode">file</span>
        <span class="meta-divider"></span>
          <span>95 lines (73 sloc)</span>
          <span class="meta-divider"></span>
        <span>3.445 kb</span>
      </div>
      <div class="actions">
        <div class="button-group">
            <a class="minibutton tooltipped tooltipped-w"
               href="http://windows.github.com" aria-label="Open this file in GitHub for Windows">
                <span class="octicon octicon-device-desktop"></span> Open
            </a>
                <a class="minibutton tooltipped tooltipped-n js-update-url-with-hash"
                   aria-label="Clicking this button will automatically fork this project so you can edit the file"
                   href="/ChibiOS/ChibiOS-RT/edit/master/os/various/devices_lib/lcd/lcd3310.h"
                   data-method="post" rel="nofollow">Edit</a>
          <a href="/ChibiOS/ChibiOS-RT/raw/master/os/various/devices_lib/lcd/lcd3310.h" class="button minibutton " id="raw-url">Raw</a>
            <a href="/ChibiOS/ChibiOS-RT/blame/master/os/various/devices_lib/lcd/lcd3310.h" class="button minibutton js-update-url-with-hash">Blame</a>
          <a href="/ChibiOS/ChibiOS-RT/commits/master/os/various/devices_lib/lcd/lcd3310.h" class="button minibutton " rel="nofollow">History</a>
        </div><!-- /.button-group -->

            <a class="minibutton danger empty-icon tooltipped tooltipped-s"
               href="/ChibiOS/ChibiOS-RT/delete/master/os/various/devices_lib/lcd/lcd3310.h"
               aria-label="Fork this project and delete file"
               data-method="post" data-test-id="delete-blob-file" rel="nofollow">

          Delete
        </a>
      </div><!-- /.actions -->
    </div>
        <div class="blob-wrapper data type-c js-blob-data">
        <table class="file-code file-diff tab-size-8">
          <tr class="file-code-line">
            <td class="blob-line-nums">
              <span id="L1" rel="#L1">1</span>
<span id="L2" rel="#L2">2</span>
<span id="L3" rel="#L3">3</span>
<span id="L4" rel="#L4">4</span>
<span id="L5" rel="#L5">5</span>
<span id="L6" rel="#L6">6</span>
<span id="L7" rel="#L7">7</span>
<span id="L8" rel="#L8">8</span>
<span id="L9" rel="#L9">9</span>
<span id="L10" rel="#L10">10</span>
<span id="L11" rel="#L11">11</span>
<span id="L12" rel="#L12">12</span>
<span id="L13" rel="#L13">13</span>
<span id="L14" rel="#L14">14</span>
<span id="L15" rel="#L15">15</span>
<span id="L16" rel="#L16">16</span>
<span id="L17" rel="#L17">17</span>
<span id="L18" rel="#L18">18</span>
<span id="L19" rel="#L19">19</span>
<span id="L20" rel="#L20">20</span>
<span id="L21" rel="#L21">21</span>
<span id="L22" rel="#L22">22</span>
<span id="L23" rel="#L23">23</span>
<span id="L24" rel="#L24">24</span>
<span id="L25" rel="#L25">25</span>
<span id="L26" rel="#L26">26</span>
<span id="L27" rel="#L27">27</span>
<span id="L28" rel="#L28">28</span>
<span id="L29" rel="#L29">29</span>
<span id="L30" rel="#L30">30</span>
<span id="L31" rel="#L31">31</span>
<span id="L32" rel="#L32">32</span>
<span id="L33" rel="#L33">33</span>
<span id="L34" rel="#L34">34</span>
<span id="L35" rel="#L35">35</span>
<span id="L36" rel="#L36">36</span>
<span id="L37" rel="#L37">37</span>
<span id="L38" rel="#L38">38</span>
<span id="L39" rel="#L39">39</span>
<span id="L40" rel="#L40">40</span>
<span id="L41" rel="#L41">41</span>
<span id="L42" rel="#L42">42</span>
<span id="L43" rel="#L43">43</span>
<span id="L44" rel="#L44">44</span>
<span id="L45" rel="#L45">45</span>
<span id="L46" rel="#L46">46</span>
<span id="L47" rel="#L47">47</span>
<span id="L48" rel="#L48">48</span>
<span id="L49" rel="#L49">49</span>
<span id="L50" rel="#L50">50</span>
<span id="L51" rel="#L51">51</span>
<span id="L52" rel="#L52">52</span>
<span id="L53" rel="#L53">53</span>
<span id="L54" rel="#L54">54</span>
<span id="L55" rel="#L55">55</span>
<span id="L56" rel="#L56">56</span>
<span id="L57" rel="#L57">57</span>
<span id="L58" rel="#L58">58</span>
<span id="L59" rel="#L59">59</span>
<span id="L60" rel="#L60">60</span>
<span id="L61" rel="#L61">61</span>
<span id="L62" rel="#L62">62</span>
<span id="L63" rel="#L63">63</span>
<span id="L64" rel="#L64">64</span>
<span id="L65" rel="#L65">65</span>
<span id="L66" rel="#L66">66</span>
<span id="L67" rel="#L67">67</span>
<span id="L68" rel="#L68">68</span>
<span id="L69" rel="#L69">69</span>
<span id="L70" rel="#L70">70</span>
<span id="L71" rel="#L71">71</span>
<span id="L72" rel="#L72">72</span>
<span id="L73" rel="#L73">73</span>
<span id="L74" rel="#L74">74</span>
<span id="L75" rel="#L75">75</span>
<span id="L76" rel="#L76">76</span>
<span id="L77" rel="#L77">77</span>
<span id="L78" rel="#L78">78</span>
<span id="L79" rel="#L79">79</span>
<span id="L80" rel="#L80">80</span>
<span id="L81" rel="#L81">81</span>
<span id="L82" rel="#L82">82</span>
<span id="L83" rel="#L83">83</span>
<span id="L84" rel="#L84">84</span>
<span id="L85" rel="#L85">85</span>
<span id="L86" rel="#L86">86</span>
<span id="L87" rel="#L87">87</span>
<span id="L88" rel="#L88">88</span>
<span id="L89" rel="#L89">89</span>
<span id="L90" rel="#L90">90</span>
<span id="L91" rel="#L91">91</span>
<span id="L92" rel="#L92">92</span>
<span id="L93" rel="#L93">93</span>
<span id="L94" rel="#L94">94</span>

            </td>
            <td class="blob-line-code"><div class="code-body highlight"><pre><div class='line' id='LC1'><span class="cm">/*</span></div><div class='line' id='LC2'><span class="cm">    Copyright (C) 2013 Marcin Jokel</span></div><div class='line' id='LC3'><br/></div><div class='line' id='LC4'><span class="cm">    Licensed under the Apache License, Version 2.0 (the &quot;License&quot;);</span></div><div class='line' id='LC5'><span class="cm">    you may not use this file except in compliance with the License.</span></div><div class='line' id='LC6'><span class="cm">    You may obtain a copy of the License at</span></div><div class='line' id='LC7'><br/></div><div class='line' id='LC8'><span class="cm">        http://www.apache.org/licenses/LICENSE-2.0</span></div><div class='line' id='LC9'><br/></div><div class='line' id='LC10'><span class="cm">    Unless required by applicable law or agreed to in writing, software</span></div><div class='line' id='LC11'><span class="cm">    distributed under the License is distributed on an &quot;AS IS&quot; BASIS,</span></div><div class='line' id='LC12'><span class="cm">    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.</span></div><div class='line' id='LC13'><span class="cm">    See the License for the specific language governing permissions and</span></div><div class='line' id='LC14'><span class="cm">    limitations under the License.</span></div><div class='line' id='LC15'><span class="cm">*/</span></div><div class='line' id='LC16'><br/></div><div class='line' id='LC17'><span class="cm">/**</span></div><div class='line' id='LC18'><span class="cm"> * @file    lcd3310.h</span></div><div class='line' id='LC19'><span class="cm"> * @brief   Nokia 3310 LCD interface module through SPI code.</span></div><div class='line' id='LC20'><span class="cm"> *</span></div><div class='line' id='LC21'><span class="cm"> * @addtogroup lcd3310</span></div><div class='line' id='LC22'><span class="cm"> * @{</span></div><div class='line' id='LC23'><span class="cm"> */</span></div><div class='line' id='LC24'><br/></div><div class='line' id='LC25'><span class="cp">#ifndef _LCD3310_H_</span></div><div class='line' id='LC26'><span class="cp">#define _LCD3310_H_</span></div><div class='line' id='LC27'><br/></div><div class='line' id='LC28'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC29'><span class="cm">/* Driver constants.                                                         */</span></div><div class='line' id='LC30'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC31'><br/></div><div class='line' id='LC32'><span class="cp">#define LCD3310_X_RES                  84</span></div><div class='line' id='LC33'><span class="cp">#define LCD3310_Y_RES                  48</span></div><div class='line' id='LC34'><br/></div><div class='line' id='LC35'><span class="cp">#define LCD3310_FONT_X_SIZE             5</span></div><div class='line' id='LC36'><span class="cp">#define LCD3310_FONT_Y_SIZE             8</span></div><div class='line' id='LC37'><br/></div><div class='line' id='LC38'><span class="cp">#define LCD3310_SEND_CMD                0</span></div><div class='line' id='LC39'><span class="cp">#define LCD3310_SEND_DATA               1</span></div><div class='line' id='LC40'><br/></div><div class='line' id='LC41'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC42'><span class="cm">/* Driver pre-compile time settings.                                         */</span></div><div class='line' id='LC43'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC44'><br/></div><div class='line' id='LC45'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC46'><span class="cm">/* Derived constants and error checks.                                       */</span></div><div class='line' id='LC47'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC48'><br/></div><div class='line' id='LC49'><span class="cp">#if !defined(LCD3310_RES_PIN)</span></div><div class='line' id='LC50'><span class="cp">#error &quot;LCD3310_RES_PIN not defined!!!&quot;</span></div><div class='line' id='LC51'><span class="cp">#endif</span></div><div class='line' id='LC52'><br/></div><div class='line' id='LC53'><span class="cp">#if !defined(LCD3310_RES_PORT)</span></div><div class='line' id='LC54'><span class="cp">#error &quot;LCD3310_RES_PORT not defined!!!&quot;</span></div><div class='line' id='LC55'><span class="cp">#endif</span></div><div class='line' id='LC56'><br/></div><div class='line' id='LC57'><span class="cp">#if !defined(LCD3310_DC_PIN)</span></div><div class='line' id='LC58'><span class="cp">#error &quot;LCD3310_DC_PIN not defined!!!&quot;</span></div><div class='line' id='LC59'><span class="cp">#endif</span></div><div class='line' id='LC60'><br/></div><div class='line' id='LC61'><span class="cp">#if!defined(LCD3310_DC_PORT)</span></div><div class='line' id='LC62'><span class="cp">#error &quot;LCD3310_DC_PORT not defined!!!&quot;</span></div><div class='line' id='LC63'><span class="cp">#endif</span></div><div class='line' id='LC64'><br/></div><div class='line' id='LC65'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC66'><span class="cm">/* Driver data structures and types.                                         */</span></div><div class='line' id='LC67'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC68'><br/></div><div class='line' id='LC69'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC70'><span class="cm">/* Driver macros.                                                            */</span></div><div class='line' id='LC71'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC72'><br/></div><div class='line' id='LC73'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC74'><span class="cm">/* External declarations.                                                    */</span></div><div class='line' id='LC75'><span class="cm">/*===========================================================================*/</span></div><div class='line' id='LC76'><br/></div><div class='line' id='LC77'><span class="cp">#ifdef __cplusplus</span></div><div class='line' id='LC78'><span class="k">extern</span> <span class="s">&quot;C&quot;</span> <span class="p">{</span></div><div class='line' id='LC79'><span class="cp">#endif</span></div><div class='line' id='LC80'>&nbsp;&nbsp;<span class="kt">void</span> <span class="n">lcd3310Init</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">);</span></div><div class='line' id='LC81'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310WriteByte</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">data</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">cd</span><span class="p">);</span></div><div class='line' id='LC82'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310Contrast</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">contrast</span><span class="p">);</span></div><div class='line' id='LC83'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310Clear</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">);</span></div><div class='line' id='LC84'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310SetPosXY</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">x</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">y</span><span class="p">);</span></div><div class='line' id='LC85'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310WriteChar</span> <span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">ch</span><span class="p">);</span></div><div class='line' id='LC86'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310WriteText</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="k">const</span> <span class="kt">uint8_t</span> <span class="o">*</span> <span class="n">strp</span><span class="p">);</span></div><div class='line' id='LC87'>&nbsp;&nbsp;<span class="kt">void</span> <span class="nf">lcd3310RotateText</span><span class="p">(</span><span class="n">SPIDriver</span> <span class="o">*</span><span class="n">spip</span><span class="p">,</span> <span class="k">const</span> <span class="kt">uint8_t</span> <span class="o">*</span> <span class="n">strp</span><span class="p">,</span> <span class="kt">uint8_t</span> <span class="n">offset</span><span class="p">);</span></div><div class='line' id='LC88'><span class="cp">#ifdef __cplusplus</span></div><div class='line' id='LC89'><span class="p">}</span></div><div class='line' id='LC90'><span class="cp">#endif</span></div><div class='line' id='LC91'><br/></div><div class='line' id='LC92'><span class="cp">#endif </span><span class="cm">/* _LCD3310_H_ */</span><span class="cp"></span></div><div class='line' id='LC93'><br/></div><div class='line' id='LC94'><span class="cm">/** @} */</span></div></pre></div></td>
          </tr>
        </table>
  </div>

  </div>
</div>

<a href="#jump-to-line" rel="facebox[.linejump]" data-hotkey="l" class="js-jump-to-line" style="display:none">Jump to Line</a>
<div id="jump-to-line" style="display:none">
  <form accept-charset="UTF-8" class="js-jump-to-line-form">
    <input class="linejump-input js-jump-to-line-field" type="text" placeholder="Jump to line&hellip;" autofocus>
    <button type="submit" class="button">Go</button>
  </form>
</div>

        </div>

      </div><!-- /.repo-container -->
      <div class="modal-backdrop"></div>
    </div><!-- /.container -->
  </div><!-- /.site -->


    </div><!-- /.wrapper -->

      <div class="container">
  <div class="site-footer">
    <ul class="site-footer-links right">
      <li><a href="https://status.github.com/">Status</a></li>
      <li><a href="http://developer.github.com">API</a></li>
      <li><a href="http://training.github.com">Training</a></li>
      <li><a href="http://shop.github.com">Shop</a></li>
      <li><a href="/blog">Blog</a></li>
      <li><a href="/about">About</a></li>

    </ul>

    <a href="/">
      <span class="mega-octicon octicon-mark-github" title="GitHub"></span>
    </a>

    <ul class="site-footer-links">
      <li>&copy; 2014 <span title="0.05105s from github-fe135-cp1-prd.iad.github.net">GitHub</span>, Inc.</li>
        <li><a href="/site/terms">Terms</a></li>
        <li><a href="/site/privacy">Privacy</a></li>
        <li><a href="/security">Security</a></li>
        <li><a href="/contact">Contact</a></li>
    </ul>
  </div><!-- /.site-footer -->
</div><!-- /.container -->


    <div class="fullscreen-overlay js-fullscreen-overlay" id="fullscreen_overlay">
  <div class="fullscreen-container js-fullscreen-container">
    <div class="textarea-wrap">
      <textarea name="fullscreen-contents" id="fullscreen-contents" class="js-fullscreen-contents" placeholder="" data-suggester="fullscreen_suggester"></textarea>
    </div>
  </div>
  <div class="fullscreen-sidebar">
    <a href="#" class="exit-fullscreen js-exit-fullscreen tooltipped tooltipped-w" aria-label="Exit Zen Mode">
      <span class="mega-octicon octicon-screen-normal"></span>
    </a>
    <a href="#" class="theme-switcher js-theme-switcher tooltipped tooltipped-w"
      aria-label="Switch themes">
      <span class="octicon octicon-color-mode"></span>
    </a>
  </div>
</div>



    <div id="ajax-error-message" class="flash flash-error">
      <span class="octicon octicon-alert"></span>
      <a href="#" class="octicon octicon-remove-close close js-ajax-error-dismiss"></a>
      Something went wrong with that request. Please try again.
    </div>

  </body>
</html>

