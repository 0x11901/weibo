//
//  WBRegisterViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/21.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit

class WBRegisterViewController: UIViewController {
    
    /// 注册新浪微博用户接口，该接口为受限接口（只对受邀请的合作伙伴开放）。
    lazy var leftBarButtonItem: UIBarButtonItem = UIBarButtonItem(title: "取消", style: .plain, target: self, action: #selector(cancel))
    lazy var webView: UIWebView = {
        let webView = UIWebView(frame: self.view.bounds)
        webView.scrollView.bounces = false
        webView.scrollView.backgroundColor = UIColor.colorWithHex(hex: 0xEBEDEF)
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

extension WBRegisterViewController {
    func setupUI() {
        navigationItem.leftBarButtonItem = leftBarButtonItem
        view.addSubview(webView)
        
        
        let urlStr = "https://m.weibo.cn/reg/index?vt=4&res=wel&wm=3349&backURL=http%3A%2F%2Fm.weibo.cn%2F%3F%26jumpfrom%3Dweibocom"
        if let url = URL(string: urlStr) {
            webView.loadRequest(URLRequest(url: url))
        }
    }
}

extension WBRegisterViewController {
    @objc fileprivate func cancel () {
        dismiss(animated: true, completion: nil)
    }
}
