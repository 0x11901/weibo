//
//  WBControlView.swift
//  weiBo
//
//  Created by 王靖凯 on 2017/11/25.
//  Copyright © 2017年 王靖凯. All rights reserved.
//

import UIKit
import SnapKit

class WBControlView: UITabBar {
    
    public var callBack: ((Int)-> Void)? = nil
    
    /// 动画的基准时间
    private let animationDuration = 0.45
    
    private lazy var backgroundView: UIScrollView = {
        let s = UIScrollView()
        s.contentSize = CGSize(width: screenWidth * 2, height: screenHeight)
        s.delegate = self
        s.isPagingEnabled = true
        s.bounces = false
        s.showsVerticalScrollIndicator = false
        s.showsHorizontalScrollIndicator = false
        return s
    }()
    
    private lazy var dayLabel: UILabel = {
        let l = UILabel(title: "8", fontSize: 50, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return l
    }()
    
    private lazy var weekdayLabel: UILabel = {
        let w = UILabel(title: "星期六", fontSize: 15, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return w
    }()
    
    private lazy var monthAndYearLabel: UILabel = {
        let m = UILabel(title: "6/2016", fontSize: 15, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return m
    }()
    
    private lazy var cityInfo: UILabel = {
        let c = UILabel(title: "", fontSize: 17, fontColor: UIColor.colorWithHex(hex: 0x666666))
        return c
    }()
    
    private lazy var animation: [UIImage] = {
        var images = [UIImage]()
        for i in 1...23 {
            if let image = UIImage(named: "compose_weather_guide_anim_\(i)") {
                images.append(image)
            }
        }
        return images
    }()
    
    /// 天气右侧动画按钮
    private lazy var compose: UIImageView = {
        let i = UIImageView()
        i.animationImages = animation
        i.animationDuration = 1
        i.animationRepeatCount = Int.max
        i.isHidden = true
        return i
    }()
    
    private lazy var page: UIPageControl = {
        let p = UIPageControl()
        p.numberOfPages = 2
        p.currentPage = 0
        p.currentPageIndicatorTintColor = globalColor
        p.pageIndicatorTintColor = UIColor.colorWithHex(hex: 0xC7C7C7)
        return p
    }()
    
    /// 关闭按钮
    private lazy var addButton: UIButton = {
        let add = UIButton(image: "tabbar_compose_icon_cancel", target: self, action: #selector(close(sender:)))
        return add
    }()
    
    /// 中间的16个按钮
    private lazy var btns: [WBControlButton] = {
        var btns = [WBControlButton]()
        guard let array = WBControlButtonModel.getControlButtonArray() else {
            console.debug("WBControlButtonModel.getControlButtonArray error")
            return btns
        }
        for item in array.enumerated() {
            let element = item.element
            let offset = item.offset
            let btn = WBControlButton()
            btn.model = element
            btn.tag = offset
            btn.addTarget(self, action: #selector(buttonHighlight(sender:)), for: .touchDown)
            btn.addTarget(self, action: #selector(buttonHighlight(sender:)), for: .touchDragEnter)
            btn.addTarget(self, action: #selector(buttonAction(sender:)), for: .touchUpInside)
            btn.addTarget(self, action: #selector(buttonCancel(sender:)), for: .touchDragExit)
            btns.append(btn)
        }
        return btns
    }()
    
    /// 正在变大的图片
    private var biggerImage: UIView?
    
    override init(frame: CGRect) {
        super.init(frame: frame)
        setupUI()
        addAnimation()
        addGestureRecognizer()
    }
    
    required init?(coder aDecoder: NSCoder) {
        fatalError("init(coder:) has not been implemented")
    }
    
}

extension WBControlView {
    
    private func setupUI() {
        addSubview(backgroundView)
        backgroundView.snp.makeConstraints { (make) in
            make.edges.equalTo(self)
        }
        
        let today = Date.today()
        
        addSubview(dayLabel)
        addSubview(weekdayLabel)
        addSubview(monthAndYearLabel)
        
        if let day = today.day {
            dayLabel.text = day
        }
        if let weekday = today.weekday {
            weekdayLabel.text = weekday
        }
        if let monthAndYear = today.monthAndYear {
            monthAndYearLabel.text = monthAndYear
        }
        
        dayLabel.snp.makeConstraints { (make) in
            make.leading.equalTo(22)
            if #available(iOS 11, *) {
                make.top.equalTo(self.safeAreaLayoutGuide.snp.topMargin).offset(100)
            }else{
                make.top.equalTo(self).offset(100)
            }
        }
        
        weekdayLabel.snp.makeConstraints { (make) in
            make.top.equalTo(dayLabel)
            make.leading.equalTo(dayLabel.snp.trailing).offset(15)
        }
        monthAndYearLabel.snp.makeConstraints { (make) in
            make.bottom.equalTo(dayLabel)
            make.leading.equalTo(weekdayLabel)
        }
        
        addSubview(cityInfo)
        addSubview(compose)
        
        cityInfo.snp.makeConstraints { (make) in
            make.leading.equalTo(dayLabel)
            make.top.equalTo(dayLabel.snp.bottom).offset(25)
        }
        compose.snp.makeConstraints { (make) in
            make.centerY.equalTo(cityInfo)
            make.leading.equalTo(cityInfo.snp.trailing).offset(10)
        }
        if let weatherInfo = WBUserInfo.shared.weatherInfo,
            let name = weatherInfo.location?.name,
            let text = weatherInfo.now?.text,
            let temperature = weatherInfo.now?.temperature {
            cityInfo.text = String(format: "%@：%@ %d°C", arguments: [name,text,temperature])
            compose.isHidden = false
        }
        compose.startAnimating()
        
        // 此处添加关闭按钮
        addSubview(addButton)
        addButton.snp.makeConstraints { (make) in
            make.centerX.equalTo(self)
            if screenHeight == 812.0 {
                make.centerY.equalTo(self.snp.bottom).offset(0.0 - (34.0 + 24.5))
            }else{
                make.centerY.equalTo(self.snp.bottom).offset(-24.5)
            }
        }
        
        // 此处添加分页按钮
        addSubview(page)
        page.snp.makeConstraints { (make) in
            make.centerY.equalTo(addButton.snp.centerY).offset(-100)
            make.centerX.equalTo(self)
        }
        
        addButtonFlow()
    }
    
    private func addButtonFlow() {
        
        let w = (screenWidth - globalMargin * 5) / 4
        let h = (screenWidth - globalMargin * 5) / 4 + 3 * margin
        
        for btn in btns.enumerated() {
            let element = btn.element
            let offset = btn.offset
            backgroundView.addSubview(element)
            element.frame.size = CGSize(width: w, height: h)
            if offset < 4 {
                element.frame.origin = CGPoint(x: globalMargin + (globalMargin + w) * CGFloat(offset), y: (screenHeight - h) / 2)
            }else if offset < 8 {
                element.frame.origin = CGPoint(x: globalMargin + (globalMargin + w) * CGFloat(offset - 4), y: (screenHeight + h) / 2)
            }else if offset < 12 {
                element.frame.origin = CGPoint(x: globalMargin + screenWidth + (globalMargin + w) * CGFloat(offset - 8), y: (screenHeight - h) / 2)
            }else{
                element.frame.origin = CGPoint(x: globalMargin + screenWidth + (globalMargin + w) * CGFloat(offset - 12), y: (screenHeight + h) / 2)
            }
        }
    }
    
    private func addGestureRecognizer() {
        let tap =  UITapGestureRecognizer(target: self, action: #selector(tapAction(sender:)))
        tap.delegate = self
        self.addGestureRecognizer(tap)
    }
}

extension WBControlView:UIGestureRecognizerDelegate {
     func gestureRecognizer(_ gestureRecognizer: UIGestureRecognizer, shouldReceive touch: UITouch) -> Bool {
        if let res = touch.view?.isMember(of: WBControlButton.self) {
            if res == true {
                return false
            }
            return true
        }
        return true
    }
}

// MARK: - 动画相关
extension WBControlView {
    
    private func addAnimation() {
        //添加扭动动画
        UIView.animate(withDuration: animationDuration) {
            self.addButton.transform = CGAffineTransform(rotationAngle: CGFloat.pi / 4)
        }
        for i in 0..<8 {
            btns[i].transform = CGAffineTransform(translationX: 0, y: screenHeight * 0.666)
            btns[i].alpha = 0.65
        }
        for i in 0..<8 {
            UIView.animate(withDuration: animationDuration * 0.5, delay: TimeInterval(Double(i) / 20.0), options: UIViewAnimationOptions.showHideTransitionViews, animations: {
                self.btns[i].transform = CGAffineTransform.identity
                self.btns[i].alpha = 1
            }, completion: { (_) in
                let col = i % 4
                let anime = CAKeyframeAnimation(keyPath: "transform.translation.y")
                anime.duration = self.animationDuration * 0.5 + 0.1 * Double(col)
                anime.repeatCount = 1
                let cave = 10.0 + Double(col) * 8
                anime.values = [0,-cave,cave * 0.5,0]
                self.btns[i].layer.add(anime, forKey: "translateUp")
            })
        }
    }
    
    private func fadeAnime() {
        for i in 0...1 {
            for j in 0..<8 {
                let delay = Double(7 - j) / 20.0
                UIView.animate(withDuration: animationDuration, delay: delay, options: UIViewAnimationOptions.showHideTransitionViews, animations: {
                    self.btns[j + i * 8].transform = CGAffineTransform(translationX: 0, y: screenHeight * 0.666)
                    self.btns[j + i * 8].alpha = 0.65
                }, completion: { (_) in
                    
                })
            }
        }

    }
    
    private func closeAnime(completion: ((Bool) -> Void)?) {
        UIView.animate(withDuration: animationDuration, animations: {
            self.addButton.transform = CGAffineTransform.identity
        }, completion: completion)
    }
    
    private func biggerAnime(sender: UIView) {
        UIView.animate(withDuration: animationDuration / 2) {
            sender.transform = CGAffineTransform(scaleX: 1.2, y: 1.2)
        }
    }
    
    private func smallAnime(sender: UIView) {
        UIView.animate(withDuration: animationDuration / 2) {
            sender.transform = CGAffineTransform.identity
        }
    }
}

extension WBControlView {
    @objc private func tapAction(sender: UITapGestureRecognizer) {
        //添加扭动动画
        fadeAnime()
        closeAnime { (_) in
            self.removeFromSuperview()
        }
    }
    
    @objc private func close(sender: UIButton) {
        //添加扭动动画
        fadeAnime()
        closeAnime { (_) in
            self.removeFromSuperview()
        }
    }
    
    @objc private func buttonAction(sender: UIControl) {
        //不知道为什么写这几行，总之写上不会崩
        for btn in btns {
            btn.isUserInteractionEnabled = false
        }
        if let tap = self.gestureRecognizers?.first {
            self.removeGestureRecognizer(tap)
        }
        UIView.animate(withDuration: animationDuration / 2, animations: {
            sender.transform = CGAffineTransform(scaleX: 2, y: 2)
            sender.alpha = 0.1
        }) { (bool) in
            if let btn = sender as? WBControlButton,let index = self.btns.index(of: btn) {
                if index == 0 {
                    self.callBack?(index)
                    self.removeFromSuperview()
                    return
                }
            }
            let btn = sender as! WBControlButton
            console.debug("你点击了\((btn.model?.title)!),然后什么也不会发生")
        }
    }
    
    @objc private func buttonHighlight(sender: UIControl) {
        // 播放变大动画
        for sub in sender.subviews {
            if sub.isMember(of: UIImageView.self) {
                if self.biggerImage != nil {
                    smallAnime(sender: self.biggerImage!)
                }
                biggerImage = sub
                biggerAnime(sender: sub)
            }
        }
    }
    
    @objc private func buttonCancel(sender: UIControl) {
        if let bigger = self.biggerImage {
            smallAnime(sender: bigger)
        }
    }
}

// MARK: - UIScrollViewDelegate
extension WBControlView: UIScrollViewDelegate {
    func scrollViewDidScroll(_ scrollView: UIScrollView) {
        let x = scrollView.contentOffset.x / screenWidth
        if x > 0.5 {
            if page.currentPage == 0 {
                page.currentPage = 1
            }
        }else{
            if page.currentPage == 1 {
                page.currentPage = 0
            }
        }
    }
}
