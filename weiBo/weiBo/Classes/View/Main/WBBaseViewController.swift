//
//  WBMainViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

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
}

// MARK: - UI相关
extension WBBaseViewController {
    func setupUI () {
        setupTableView()
        if WBUserAccountModel.shared.isLogin != true {
            setupVisitorView ()
        }
    }
    
    func setupTableView () {
        view.addSubview(tableView)
        tableView.dataSource = self
        tableView.delegate = self
    }

    func setupVisitorView () {
        visitorView = WBVisitorView(frame: self.view.bounds)
        visitorView?.delegate = self
        view.addSubview(visitorView!)
    }

}

// MARK: - 响应事件
extension WBBaseViewController {
    
    @objc fileprivate func whenLoginIsSucess() {
        if let visitorView = visitorView{
            visitorView.removeFromSuperview()
        }
        visitorView = nil
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
    
}

// MARK: - WBVisitorViewDelegate
extension WBBaseViewController: WBVisitorViewDelegate {
    func didClickedLogin() {
        let web = WBOAuthViewController()
        let navi = UINavigationController(rootViewController: web)
        present(navi, animated: true, completion: nil)
    }
}

