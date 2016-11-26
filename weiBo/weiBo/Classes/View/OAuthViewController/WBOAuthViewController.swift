//
//  WBOAuthViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/26.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBOAuthViewController: UIViewController {
    lazy var leftBarButtonItem: UIBarButtonItem = UIBarButtonItem(title: "取消", style: .plain, target: self, action: #selector(cancel))
    lazy var webView: UIWebView = {
        let webView = UIWebView(frame: self.view.bounds)
        webView.delegate = self
        return webView
    }()
    
    override func viewDidLoad() {
        super.viewDidLoad()
        // Do any additional setup after loading the view.
        setupUI()
    }
    
    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
}

extension WBOAuthViewController {
    func setupUI() {
        navigationItem.leftBarButtonItem = leftBarButtonItem
        view.addSubview(webView)
        
        let urlStr = "https://api.weibo.com/oauth2/authorize?client_id=\(appKey)&redirect_uri=\(redirectURI)"
        if let url = URL(string: urlStr) {
            webView.loadRequest(URLRequest(url: url))
        }
    }
}

extension WBOAuthViewController {
    @objc fileprivate func cancel () {
        dismiss(animated: true, completion: nil)
    }
}

extension WBOAuthViewController: UIWebViewDelegate {
    func webView(_ webView: UIWebView, shouldStartLoadWith request: URLRequest, navigationType: UIWebViewNavigationType) -> Bool {
        if let url = request.url?.absoluteString {
            if url.hasPrefix(redirectURI) {
                if let query = request.url?.query {
                    if query.hasPrefix("code=") {
                        let code = query.substring(from: "code=".endIndex)
                        let parameters = ["client_id": appKey,
                                          "client_secret": appSecrect,
                                          "grant_type": "authorization_code",
                                          "code": code,
                                          "redirect_uri": redirectURI]
                        
                        NetworkTool.shared.POST(URLString: "https://api.weibo.com/oauth2/access_token", parameters: parameters, success: { (response) in
                            print(response!)
                        }, failure: { (error) in
                            print(error)
                        })
                        cancel()
                    }else{
                        cancel()
                    }
                }
                return false
            }
        }
        return true
    }
    
    func webViewDidFinishLoad(_ webView: UIWebView) {
        webView.stringByEvaluatingJavaScript(from: "document.getElementById('userId').value = '627515277@qq.com'; document.getElementById('passwd').value = 'Wx27ueBPv8srIc';")
    }
}
