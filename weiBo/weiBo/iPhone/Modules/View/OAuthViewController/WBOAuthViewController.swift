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
        webView.scrollView.bounces = false
        webView.scrollView.backgroundColor = UIColor.colorWithHex(hex: 0xEBEDEF)
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
    
    deinit {
        NotificationCenter.default.removeObserver(self)
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
                        let code = String(query["code=".endIndex...])
                        
                        NetworkManager.shared.requestForAccessToken(code: code, networkCompletionHandler: { (obj) in
                            guard let dictionary = obj, let access_token = dictionary["access_token"],let uid = dictionary["uid"] else {
                                console.debug("没有获得正确的access_token信息")
                                self.cancel()
                                return
                            }
                            let parameters = ["access_token": access_token,"uid": uid]
                            NetworkManager.shared.requestForUserInfo(parameters: parameters, networkCompletionHandler: { (obj) in
                                guard var responseObject = obj else {
                                    console.debug("没有获得正确的userAccount信息")
                                    self.cancel()
                                    return
                                }
                                
                                for (key,value) in dictionary {
                                    responseObject[key] = value
                                }
                                
                                
                                WBUserAccountModel.shared.saveAccount(dictionary: responseObject)
                                NotificationCenter.default.post(name: NSNotification.Name(rawValue: loginSuccess), object: self)
                                
                                self.cancel()
                                // TODO: 如果登陆失败的提示没做
                            })
                        })
                        
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
        webView.stringByEvaluatingJavaScript(from: "document.getElementById('userId').value = '627515277@qq.com'")
    }
}
