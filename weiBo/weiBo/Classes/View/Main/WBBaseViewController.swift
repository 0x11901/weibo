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
    var visitorView: WBVisitorView?
    
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

extension WBBaseViewController {
    func setupUI () {
        setupTableView()
        setupVisitorView()
    }
    
    func setupTableView () {
        view.addSubview(tableView)
        tableView.dataSource = self
        tableView.delegate = self
    }

    func setupVisitorView () {
        visitorView = WBVisitorView(frame: self.view.bounds)
        view.addSubview(visitorView!)
    }

}

extension WBBaseViewController: UITableViewDataSource {
    func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int{
        return 0
    }

    func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell{
        return UITableViewCell()
    }
}

extension WBBaseViewController: UITableViewDelegate {
    
}
