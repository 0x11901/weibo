//
//  WBHomeViewController.swift
//  weiBo
//
//  Created by 王靖凯 on 2016/11/24.
//  Copyright © 2016年 王靖凯. All rights reserved.
//

import UIKit

class WBHomeViewController: WBBaseViewController {
    lazy var listViewModel = WBStatusListViewModel()
    let cellReuseIdentifier = "dasfsdfsfsf"
    
    override func viewDidLoad() {
        super.viewDidLoad()
        loadStatus(isPull: true)
        // Do any additional setup after loading the view.
        NotificationCenter.default.addObserver(self, selector: #selector(didSelectedAnImage(sender:)), name: clickThumbImage, object: nil)
    }

    deinit {
        NotificationCenter.default.removeObserver(self)
    }
    
}

// MARK: - 读取数据
extension WBHomeViewController {
    fileprivate func loadStatus(isPull: Bool) {
        listViewModel.loadDate(isPull: isPull) { (success) in
            if success {
                if isPull {
                    self.header.endRefreshing()
                }else{
                    self.footer.endRefreshing()
                }
                self.tableView.reloadData()
            }
        }
    }
}

// MARK: - reloadData
extension WBHomeViewController {
    
    override func setupTableView() {
        super.setupTableView()
        tableView.register(WBStatusCell.self, forCellReuseIdentifier: cellReuseIdentifier)
        tableView.estimatedRowHeight = 200
        tableView.rowHeight = UITableViewAutomaticDimension
    }
    
    override func tableView(_ tableView: UITableView, numberOfRowsInSection section: Int) -> Int {
        return listViewModel.dataSource.count
    }
    
    override func tableView(_ tableView: UITableView, cellForRowAt indexPath: IndexPath) -> UITableViewCell {
        let cell = tableView.dequeueReusableCell(withIdentifier: cellReuseIdentifier, for: indexPath) as! WBStatusCell
        cell.status = listViewModel.dataSource[indexPath.row]
        return cell
    }
    
    override func headerRefresh() {
        loadStatus(isPull: true)
    }
    
    override func footerRefresh() {
        loadStatus(isPull: false)
    }
}

extension WBHomeViewController {
    
    @objc fileprivate func didSelectedAnImage(sender: Notification) {
        print(sender)
    }
    
}
