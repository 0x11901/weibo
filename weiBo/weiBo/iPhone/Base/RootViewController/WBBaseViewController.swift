//
//  WBMainViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit
import MJRefresh

class WBBaseViewController: UIViewController {
    lazy var tableView: UITableView = UITableView(frame: self.view.bounds, style: .plain)
    var model: WBVisitorModel?
    var visitorView: WBVisitorView?{
        didSet{
            if let message = model?.message {
                visitorView?.textLable.text = message
            }
            if let imageName = model?.imageName {
                visitorView?.iconImage.image = UIImage(named: imageName)
            }
            if let anima = model?.isAnima {
                if anima == true {
                    visitorView?.circleImage.isHidden = !anima
                }else{
                    visitorView?.circleImage.isHidden = !anima
                }
            }
        }
    }
    lazy var header: MJRefreshNormalHeader = MJRefreshNormalHeader(refreshingTarget: self, refreshingAction: #selector(headerRefresh))
    lazy var footer: MJRefreshAutoNormalFooter = MJRefreshAutoNormalFooter(refreshingTarget: self, refreshingAction: #selector(footerRefresh))
    
    override func viewDidLoad() {
        super.viewDidLoad()

        // Do any additional setup after loading the view.
        setupUI()
        NotificationCenter.default.addObserver(self, selector: #selector(whenLoginIsSucess), name: NSNotification.Name(rawValue: loginSuccess), object: nil)
    }

    override func didReceiveMemoryWarning() {
        super.didReceiveMemoryWarning()
        // Dispose of any resources that can be recreated.
    }
    
    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
    
    func setupUI () {
        setupTableView()
        if WBUserAccountModel.shared.isLogin != true {
            setupVisitorView ()
        }
    }
    
    func setupTableView () {
        view.addSubview(tableView)
        tableView.separatorStyle = .none
        tableView.dataSource = self
        tableView.delegate = self
        tableView.mj_header = header
        tableView.mj_footer = footer
    }
    
    func setupVisitorView () {
        visitorView = WBVisitorView(frame: self.view.bounds)
        visitorView?.delegate = self
        view.addSubview(visitorView!)
    }

}

// MARK: - UI相关
extension WBBaseViewController {
    
}

// MARK: - 响应事件
extension WBBaseViewController {
    
    @objc func whenLoginIsSucess() {
        if let visitorView = visitorView{
            visitorView.removeFromSuperview()
        }
        visitorView = nil
        
    }
    
    @objc func headerRefresh() {
    }
    
    @objc func footerRefresh() {
    }
    
}

// MARK: - UITableViewDataSource
extension WBBaseViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int{
        return 0
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell{
        return UITableViewCell()
    }
}

// MARK: - UITableViewDelegate
extension WBBaseViewController: UITableViewDelegate {
    func tableView(_ tableView: UITableView, didSelectRowAt indexPath: IndexPath) {
        tableView.deselectRow(at: indexPath, animated: true)
    }
}

// MARK: - WBVisitorViewDelegate
extension WBBaseViewController: WBVisitorViewDelegate {
    func didClickedLogin() {
        let web = WBOAuthViewController()
        let navi = UINavigationController(rootViewController: web)
        present(navi, animated: true, completion: nil)
    }
    
    func didClickedRegister() {
        let web = WBRegisterViewController()
        let navi = UINavigationController(rootViewController: web)
        present(navi, animated: true, completion: nil)
    }
}

